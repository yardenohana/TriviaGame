import socket
import json

SERVER_IP = '127.0.0.1'
SERVER_PORT = 8200
SERVER_MSG_BUFF = 1024  # Number of bytes to get from the server message

#  code being sent to the server:
SIGNUP_CODE = 100
LOGIN_CODE = 101
GET_PLAYERS = 102
JOIN_ROOM = 103
CREATE_ROOM  = 104
GET_HIGHSCORE = 105
LOGOUT_CODE = 106
GET_ROOM = 107

# codes being used by server reponses:
SIGNUP_RESPONSE = 202
LOGIN_RESPONSE = 201
ERROR_REPONSE = 200

# A class represents the response of the server,
# It is used to retrieve data from it easily.
class response:
    code = 0
    len = 0
    js = ''

    def __init__(self, server_msg):
        self.code = server_msg[0]
        self.len = server_msg[1] + server_msg [2] +\
                   server_msg[3] + server_msg[4]
        self.js = server_msg[5:]

    def getCode(self):
        return self.code;

    def getLen(self):
        return self.len

    def getJson(self):
        return self.js


def TextToBinaryString(words):
    res = ''.join(format(ord(i), 'b') for i in words)
    return res


def generateMessage(ans):
    """
    :param ans: 1 for login, 2 for signup
    :type ans: int
    :return: str
    """
    msg = ''  # the message sent to the server
    code = 0  # represents the code of message - login/signup
    username = input("enter your username: ")  # enter "user1"
    password = input("enter your password: ")  # enter "user1"

    if ans == 2:  # singup
        code =  SIGNUP_CODE  # 101
        email = input("enter your email: ")
        msg = '{"username": "' + username + '", "password": "' + password + '", "email": "' + email + '"}'
        print(msg)
    elif ans == 1:  # login
        code = LOGIN_CODE  # 100
        msg = '{"username": "' + username + '", "password": "' + password + '"}'

    y = json.loads(msg)
    length = len(msg)

    client_msg = chr(code)
    for byte in length.to_bytes(4, byteorder='big'):
        if int(byte) == 0:
            client_msg += ' '
        else:
            client_msg += chr(int(byte))

    client_msg += str(y)
    return client_msg


def room_handler():
    """
    The function handles a room request of any kind
    :return: str
    """
    msg_code = 0
    final_msg = ""  # the str message request

    choice = int(input("Enter 1 to create room, 2 to join room, 3 to get players, 4 to get rooms: "))
    if choice == 1:
        msg_code = CREATE_ROOM
        final_msg += chr(msg_code)
        room_name = input("Enter rooms name: ")
        max_users = input("Enter maximum users in the room: ")
        num_of_questions = input("Enter number of questions: ")
        answer_timeout = input("Enter answer timeout: ")

        data = '{"roomName": "' + room_name + '", "maxUsers": ' + max_users\
                + ', "questionCount": ' + num_of_questions + ', "answerTimeout": ' + answer_timeout\
                + '}'

        length = len(data)
        y = json.loads(data)

        for byte in length.to_bytes(4, byteorder='big'):  # add length, 4 bytes in message
            if int(byte) == 0:
                final_msg += ' '
            else:
                final_msg += chr(int(byte))

        final_msg += str(y)  # data

    elif choice == 2:
        msg_code = JOIN_ROOM
        final_msg += chr(msg_code)
        room_id = input("Enter room id: ")
        data = '{"roomID": "' + room_id + '"}'
        length = len(data)
        y = json.loads(data)

        for byte in length.to_bytes(4, byteorder='big'):  # add length, 4 bytes in message
            if int(byte) == 0:
                final_msg += ' '
            else:
                final_msg += chr(int(byte))

        final_msg += str(y)  # data

    elif choice == 3:
        msg_code = GET_PLAYERS
        final_msg += chr(msg_code)
        room_id = input("Enter room id: ")
        data = '{"roomID": "' + room_id + '"}'
        length = len(data)
        y = json.loads(data)

        for byte in length.to_bytes(4, byteorder='big'):  # add length, 4 bytes in message
            if int(byte) == 0:
                final_msg += ' '
            else:
                final_msg += chr(int(byte))

        final_msg += str(y)  # data

    elif choice == 4:
        msg_code = GET_ROOM
        final_msg += chr(msg_code)
        data = '{}'
        length = len(data)
        y = json.loads(data)

        for byte in length.to_bytes(4, byteorder='big'):  # add length, 4 bytes in message
            if int(byte) == 0:
                final_msg += ' '
            else:
                final_msg += chr(int(byte))
        final_msg += str(y)  # data

    return final_msg

def main():

    try:
        # Create a TCP / IP socket
        soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # Connecting to a remote computer
        server_adress = (SERVER_IP, SERVER_PORT)
        soc.connect(server_adress)

        ans = int(input('for login enter 1, for signup enter 2: '))
        client_msg = generateMessage(ans)
        print(client_msg)

        soc.send(client_msg.encode())
        jsonAns = soc.recv(SERVER_MSG_BUFF)
        print('server answer: ' + str(jsonAns))

        msgMenu = room_handler()
        print('msgMenu:', msgMenu)
        soc.send(msgMenu.encode())

        jsonAns = soc.recv(SERVER_MSG_BUFF)
        print('server answer: ' + str(jsonAns))

        soc.close()
    except Exception as e:
        print('Error: ', e)

if __name__ == "__main__":
    main()

    #+"10143{'username': 'danaaa', 'password': 'abc123'}"

