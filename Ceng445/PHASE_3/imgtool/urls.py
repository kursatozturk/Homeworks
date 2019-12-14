from django.urls import path

from . import views

app_name = "imgtool"
urlpatterns = [
    path('', views.index, name='redirect'),
    path('home/', views.home, name='home'),
    path('register/', views.register_user, name='register'),
    path('login/', views.login_user, name='login'),
    path('logout/', views.logout_user, name='logout'),
    path('add_image/', views.add_image, name='upload'),
    path('image/<str:image_name>/', views.image, name="image"),
    path('image/<str:image_name>/change_def_act/', views.change_def_action, name="changedefact"),
    path('image/<str:image_name>/add_rule/', views.add_rule, name="rulepage"),
    path('image/<str:image_name>/del_rule/', views.del_rule, name="delrulepage"),
    path('image/<str:image_name>/del_image/', views.del_image, name="delimage"),
    path('userpage/<str:username>/', views.user, name="user"),
    path('user/<str:username>/set_password', views.set_password, name="set_password"),
    
]
