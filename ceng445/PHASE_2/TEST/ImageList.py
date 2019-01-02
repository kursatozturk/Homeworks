def ImageList(conn):
    c = conn.cursor()
    c.execute("select i.owner, i.name from LabeledImage")
    fetched = c.fetchall()
    result = []
    if not fetched:
        print("There is no image in the database")
        return result
    for group in fetched:
        result.append((group[0], group[1]))
    conn.commit()
    conn.close()
    return result