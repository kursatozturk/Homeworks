import sqlite3
from user_group import UserGroup

DB_NAME = "ImgAnnTool.db"

conn = sqlite3.connect(DB_NAME)
c = conn.cursor()

c.execute("select * from Groups")
print("\n####### GROUPS #######")
for row in c.fetchall():
    print(row)
c.execute("select * from Users")
print("\n####### USERS #######")
for row in c.fetchall():
    print(row)
c.execute("select * from isMember")
print("\n####### IS MEMBER? #######")
for row in c.fetchall():
    print(row)

conn.commit()
conn.close()