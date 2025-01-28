import sqlite3
import requests
import sys
import time


MAX_QUESTIONS = 250


def openDB():
    conn = sqlite3.connect('TriviaDB.sqlite')
    print("Opened database successfully")

    conn.execute('''create table if not exists QUESTIONS 
    (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
     QUESTION QUESTION NOT NULL,
     DIFFICULTY TEXT NOT NULL,
     CATEGORY TEXT NOT NULL)''')

    conn.execute('''create table if not exists ANSWERS 
    (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
     QUESTION_ID INTEGER NOT NULL,
     ANSWER TEXT NOT NULL,
     IS_CORRECT INTEGER NOT NULL,
     FOREIGN KEY (QUESTION_ID) REFERENCES QUESTIONS(ID))''')

    return conn


def check_valid_question(question):
    # remove question if answer or questions contain special character
    if ';' in question['question']:
        return False

    if ';' in question['correct_answer']:
        return False

    for incorrect_answer in question['incorrect_answers']:
        if ';' in incorrect_answer:
            return False

    return True


def save_questions_to_DB(conn, questions):
    for question in questions:
        print(question)

        conn.isolation_level = None
        c = conn.cursor()
        c.execute("begin")
        try:
            # insert into QUESTION TABLE
            c.execute(f"INSERT INTO QUESTIONS (QUESTION, DIFFICULTY, CATEGORY) VALUES (\'{question['question']}\', \'{question['difficulty']}\', \'{question['category']}\');")

            # insert into ANSWERS TABLE
            question_id = c.lastrowid
            # insert incorrect answers
            for answer in question['incorrect_answers']:
                c.execute(f"INSERT INTO ANSWERS (QUESTION_ID, ANSWER, IS_CORRECT) VALUES ({question_id}, \'{answer}\', {0});")

            # insert correct answer
            c.execute(f"INSERT INTO ANSWERS (QUESTION_ID, ANSWER, IS_CORRECT) VALUES ({question_id}, \'{question['correct_answer']}\', {1});")

            # commit
            c.execute("commit")
        except conn.Error as e:
            print(e)
            print("failed!")
            c.execute("rollback")


def get_number_of_questions():
    num_of_questions = 10
    if len(sys.argv) == 1:
        print(f"no question amount, default is {num_of_questions}")
    elif not sys.argv[1].isnumeric():
        print(f"question amount is not numeric, default is {num_of_questions}")
    elif int(sys.argv[1]) > MAX_QUESTIONS:
        print(f'max question number is: {MAX_QUESTIONS}')
    else:
        num_of_questions = int(sys.argv[1])

    return num_of_questions


def getQuestions(num_of_questions):
    questions = []

    while len(questions) < num_of_questions:
        r = requests.get(f'https://opentdb.com/api.php?amount={(num_of_questions - len(questions)) * 2}&category=9&type=multiple')  # because of encoding, we need to get more questions than we want because some questions have symbols we don't support
        if r.json()['response_code'] != 0:
            raise Exception("JSON NOT FOUNT")

        for question in r.json()['results']:
            if check_valid_question(question):
                questions.append(question)
                if len(questions) == num_of_questions:
                    break

        if len(questions) == num_of_questions:
                    break

        print("waiting for api limit...")
        time.sleep(5)

    return questions


def main():
    try:
        db = openDB()
        questions = getQuestions(get_number_of_questions())
        save_questions_to_DB(db, questions)
    except Exception as e:
        print(f'ERROR occurred: {e}')


if __name__ == '__main__':
    main()
