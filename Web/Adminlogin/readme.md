Challenge Name:Adminlogin
Description:
The deal is simple, I have forgotten the admin password, help me find it out, I will give you the flag
<br>
https://miniques4.herokuapp.com/
Solution:
The code shows that your password is first md5 hashed and then compared with the following string 0e514198428367523082236389979035
Now this is a classic Type juggling problem, you find a string which matches the above string after md5 hashing and then pass it in the login form.
In my case i did QNKCDZO and the site gave me the flag.
Flag: VishwaCTF{typejugglingdobehard}