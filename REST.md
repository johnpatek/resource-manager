# Resource Manager REST API

A comprehensive guide to every endpoint supported by the Resource Manager REST API

## Authentication Endpoints

<h3><p><span style="color:White;background:DodgerBlue;padding:10px;">
POST 
</span><span style="color:Black;padding:10px;">
/login
</span></p></h3>
Begin session using existing credentials.

#### Request

```json
{
    email : "johnpatek2@gmail.com",
    password : "MyPassword123"
}
```

#### Response

```json
{
    status: "ok"
    session_id: "0123456789"
}
```



<h3><p><span style="color:White;background:MediumSeaGreen;padding:10px;">
GET 
</span><span style="color:Black;padding:10px;">
/logout
</span></p></h3>
End current session.

#### Request

| Header         | Description                       |
| :------------- | :---------------------------------|
|  SessionId     | Session ID returned by `/login`.  |

#### Response

```json
{
    message: "Goodbye, johnpatek2@gmail.com!"
}
```



## User Endpoints

<h3><p><span style="color:White;background:MediumSeaGreen;padding:10px;">
GET 
</span><span style="color:Black;padding:10px;">
/users
</span></p></h3>
Returns a list of all users

#### Request

| Header         | Description                       |
| :------------- | :---------------------------------|
|  SessionId     | Session ID returned by `/login`.  |

#### Response

```json
{
    users : 
    [

    ]
}
```



<h3><p><span style="color:White;background:DodgerBlue;padding:10px;">
POST
</span><span style="color:Black;padding:10px;">
/users
</span></p></h3>
Create a new user

#### Request

| Header         | Description                       |
| :------------- | :---------------------------------|
|  SessionId     | Session ID returned by `/login`.  |

```json
```

#### Response

```json
```



<h3><p><span style="color:White;background:MediumSeaGreen;padding:10px;">
GET 
</span><span style="color:Black;padding:10px;">
/users/{userid}
</span></p></h3>
Get a user

#### Request

| Header         | Description                       |
| :------------- | :---------------------------------|
|  SessionId     | Session ID returned by `/login`.  |

#### Response

```json
```



<h3><p><span style="color:White;background:Gold;padding:10px;">
PUT 
</span><span style="color:Black;padding:10px;">
/users/{userid}
</span></p></h3>
Update a user

#### Request

| Header         | Description                       |
| :------------- | :---------------------------------|
|  SessionId     | Session ID returned by `/login`.  |



<h3><p><span style="color:White;background:Red;padding:10px;">
DELETE 
</span><span style="color:Black;padding:10px;">
/users/{userid}
</span></p></h3>
Delete a user

#### Request

| Header         | Description                       |
| :------------- | :---------------------------------|
|  SessionId     | Session ID returned by `/login`.  |



## Group Endpoints

<h3><p><span style="color:White;background:MediumSeaGreen;padding:10px;">
GET 
</span><span style="color:Black;padding:10px;">
/groups
</span></p></h3>
Returns a list of all groups



## Resource Endpoints



## Miscellaneous Endpoints