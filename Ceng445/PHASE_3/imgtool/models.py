from django.db import models
from django.contrib.auth.models import User
import base64

class Group(models.Model):
    """ Group Object """
    groupname = models.CharField(max_length=20, primary_key=True)
    def __str__(self):
        return self.groupname

class ImgAnnToolUser(models.Model):
    """ User Object """
    #username = models.CharField(max_length=20, primary_key=True)
    #password = models.CharField(max_length=20)
    user = models.OneToOneField(User, on_delete=models.CASCADE, related_name='user')
    groups = models.ManyToManyField(Group, related_name='members')
    def __str__(self):
        return self.user.username

class Image(models.Model):
    """ Image Object """
    #image_id = models.AutoField(primary_key=True)
    #image_src = models.ImageField()
    #rules = models.CharField(max_length=10000)
    name = models.CharField(max_length=100, unique=True)
    owner = models.ForeignKey(ImgAnnToolUser, on_delete=models.CASCADE, related_name='owns')
    image_src = models.BinaryField()

    def __str__(self):
        return self.name





# Image_id INT PRIMARY KEY,
#                  Object BLOB,
#                  name CHAR(50),
#                  owner CHAR(20),
#                  FOREIGN  KEY(owner) REFERENCES USERS(uname) ON DELETE CASCADE ON UPDATE CASCADE
