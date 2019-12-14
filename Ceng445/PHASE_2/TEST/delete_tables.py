import sqlite3
from user_group import UserGroup

DB_NAME = "ImgAnnTool.db"

conn = sqlite3.connect(DB_NAME)
c = conn.cursor()

c.execute("delete from Groups")
c.execute("delete from Users")
c.execute("delete from isMember")

conn.commit()
conn.close()