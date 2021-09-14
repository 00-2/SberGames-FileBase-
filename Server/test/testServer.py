import unittest
import socket

import random
import string

from struct import *

import os.path
import time

def generate_random_string(length):
    letters = string.ascii_lowercase
    rand_string = ''.join(random.choice(letters) for i in range(length))
    return(rand_string+'\n')

def test_creating_file_string(rand_string):
    conn = socket.socket()
    conn.connect( ("127.0.0.1", 8989) )
    condition=1
    #проверка идет на директории data и порту 8989 (параметры сервера не указаны)
    path=rand_string
    #если существует файл
    if(os.path.exists("../data/"+path)):
        #то удаляем
        os.remove("../data/"+path)
    size=len(path)
    #отправляем запрос на создание
    strToPack = "H ? %ss" % (size)
    msg = pack(strToPack,size,condition,bytes(path,'utf-8'))
    conn.send(msg)    
    conn.close()
    time.sleep(0.001)
    #проверяем, существует ли файл
    return os.path.exists("../data/"+path)


class TestServerMethods(unittest.TestCase):
  def test_creating_files(self):
    #проверка идет на директории data и порту 8989 (параметры сервера не указаны)
    ctd = 0
    for i in range(0,100):
        rand_string = generate_random_string(8)
        ctd += test_creating_file_string(rand_string)
    time.sleep(0.5)
    self.assertTrue(ctd > 90)

  def test_creating_file(self):
    conn = socket.socket()
    conn.connect( ("127.0.0.1", 8989) )
    condition=1
    #проверка идет на директории data и порту 8989 (параметры сервера не указаны)
    rand_string = "test.txt"
    path=rand_string
    #если существует файл
    if(os.path.exists("../data/"+path)):
        #то удаляем
        os.remove("../data/"+path)
    size=len(path)
    #отправляем запрос на создание
    strToPack = "H ? %ss" % (size)
    msg = pack(strToPack,size,condition,bytes(path,'utf-8'))
    conn.send(msg)    
    conn.close()
    time.sleep(0.001)
    #проверяем, существует ли файл
    self.assertTrue(os.path.exists("../data/"+path)) 

  def test_deleting_file(self):
    conn = socket.socket()
    conn.connect( ("127.0.0.1", 8989) )
    condition=0
    #проверка идет на директории data и порту 8989 (параметры сервера не указаны)
    path="test.txt"
    #если существует файл
    if(not os.path.exists("../data/"+path)):
        #то удаляем
        open("../data/"+path,'w')
    size=len(path)
    #отправляем запрос на создание
    strToPack = "H ? %ss" % (size)
    msg = pack(strToPack,size,condition,bytes(path,'utf-8'))
    conn.send(msg)    
    conn.close()
    time.sleep(0.001)
    #проверяем, существует ли файл
    self.assertTrue(not os.path.exists("../data/"+path)) 
     

if __name__ == '__main__':
    unittest.main()

