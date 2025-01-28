import socket
import json


def createMessage(jsonData, codeId):
    json_data = json.dumps(jsonData)

    # Convert to bytes
    json_length = len(json_data)
    packed_length = json_length.to_bytes(4, 'big')
    send = codeId + packed_length + json_data.encode()

    return send


def sendMessage(s, data_to_send, codeId):
    send = createMessage(data_to_send, codeId)
    s.sendall(send)

    # Receive response from server
    response = s.recv(1024)

    # Decode received JSON response
    received_data = json.loads(response[5:].decode())

    print("Received:", received_data)

    if "status" in received_data:
        if received_data.get("status") == 1:
            print("Server responded with success.")
    elif "message" in received_data:
        print("Server responded with a message:", received_data["message"])
    else:
        print("Unexpected response from server.")


def client():
    HOST = "127.0.0.1"
    PORT = 8826

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        try:
            s.connect((HOST, PORT))
            print("Connected to server.")

            while True:
                action = input("Do you want to sign up (S) or log in (L)? ").strip().lower()
                if action == 's' or action == 'S':
                    example_for_debug = {"username": "example_username",
                    "password": "HebrewPass8*",
                    "email": "example123@example.com",
                    "addr": {
                        "street": "Example Street",
                        "apt": 123,
                        "city": "Example City"
                    },
                    "birth": "01.01.1990",
                    "phone": "050-1234567"}

                    username = input("Enter username: ").strip()
                    password = input("Enter password: ").strip()
                    email = input("Enter email: ").strip()

                    # Collect address components separately
                    street = input("Enter street: ").strip()
                    try:
                        apt = int(input("Enter apartment number: ").strip())  # Expecting an integer for apt
                    except ValueError:
                        print("Apartment number should be an integer.")
                        apt = 0  # Or handle it differently
                    city = input("Enter city: ").strip()

                    addr = {
                        "street": street,
                        "apt": apt,
                        "city": city
                    }

                    birth = input("Enter birth date (DD.MM.YYYY): ").strip()
                    phone = input("Enter phone (XXX-XXXXXXX): ").strip()

                    # Now addr is structured as a dictionary, similar to the example
                    data_to_send = {
                        "username": username,
                        "password": password,
                        "email": email,
                        "addr": addr,
                        "birth": birth,
                        "phone": phone
                    }
                    sendMessage(s, example_for_debug, b"\x02")
                    break
                elif action == 'l' or action == 'L':
                    username = input("Enter username: ").strip()
                    password = input("Enter password: ").strip()
                    data_to_send = {"username": username, "password": password}
                    sendMessage(s, data_to_send, b"\x01")
                    break
                else:
                    print("Invalid input. Please enter 'S' to sign up or 'L' to log in.")

        except ConnectionRefusedError:
            print("Connection to server failed. Make sure the server is running.")


if __name__ == "__main__":
    client()
