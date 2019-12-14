from Client import Client
from threading import Thread





th = [Thread(target=Client.start) for i in range(10)]

for i in th:
    print('startt')
    i.start()