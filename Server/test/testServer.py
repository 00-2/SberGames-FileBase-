import unittest
import socket

import random
import string


def generate_random_string(length):
    letters = string.ascii_lowercase
    rand_string = ''.join(random.choice(letters) for i in range(length))
    return(rand_string+'\n')


class TestServerMethods(unittest.TestCase):

  def test_single_request(self):
    conn = socket.socket()
    conn.connect( ("127.0.0.1", 8989) )
    msg = b"Hello! \n"
    conn.send(msg)
    data = b""
    tmp = conn.recv(1024)
    while tmp:
        data += tmp
        tmp = conn.recv(1024)
    conn.close()
    self.assertEqual(msg.decode("utf-8").replace(
        '\n','\x00'
    )
    ,data.decode("utf-8"))

  def test_many_requests(self):
    for i in range(0,100):
        conn = socket.socket()
        conn.connect( ("127.0.0.1", 8989) )
        rand_string = generate_random_string(8)
        msg = bytes(rand_string,'utf-8')
        conn.send(msg)
        data = b""
        tmp = conn.recv(1024)
        while tmp:
            data += tmp
            tmp = conn.recv(1024)
        conn.close()
        self.assertEqual(msg.decode("utf-8").replace(
            '\n','\x00'
        )
        ,data.decode("utf-8"))


if __name__ == '__main__':
    unittest.main()

