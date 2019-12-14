from socket import *
from concurrent.futures import ThreadPoolExecutor
import pickle
import sys


def pinger(thread_num):
    with socket(AF_INET, SOCK_STREAM) as sock:
        try:

            sock.connect(('localhost', 1025))
            d = dict(
                client_name='admin',
                func='add_user',
                args= ('user{}'.format(thread_num),
                       ['group{}'.format(thread_num), 'group{}'.format(thread_num + 1), 'group{}'.format(5)],
                       'passwd')
            )
            d = pickle.dumps(d)
            size = d.__sizeof__()
            d = size.to_bytes(4, sys.byteorder) + d
            sock.sendall(d)
            resp = pickle.loads(sock.recv(1024))
            print('response is: ', resp)
            sock.close()
        except Exception as e:
            print(e)


if __name__ == '__main__':
    with ThreadPoolExecutor() as pool:
        pool.map(pinger, range(1, 100))
    print('finished')
