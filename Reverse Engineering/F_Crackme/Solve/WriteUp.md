**Language:** C/C++
**Platform:** Windows
**Difficulty:** 3.0

---

**Tools:** CFF Explorer, IDA, x64dbg

---

The program is not obfuscated, which made it a whole lot easier

Throw it straight into a disassembler: **Ida 7.6**.

The debug information is still in there. All variable names, function names, everything is still in the binary.

```cpp
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char Source[32]; // [rsp+20h] [rbp-60h] BYREF
  char Str2[32]; // [rsp+40h] [rbp-40h] BYREF
  char Destination[24]; // [rsp+60h] [rbp-20h] BYREF
  int v8; // [rsp+7Ch] [rbp-4h]

  _main(argc, argv, envp);
  message();
  printf("Type your username: ");
  scanf("%20s", Destination);
  v8 = checkUsername(Destination);
  if ( v8 )
  {
    if ( v8 != 1 )
      exit(0);
    strcpy(Source, "Mr.");
    strcat(Source, Destination);
  }
  else
  {
    strcpy(Source, "@fsociety");
    strcat(Destination, Source);
    strcpy(Source, Destination);
  }
  printf("Type your password: ");
  scanf("%30s", Str2);
  if ( strcmp(Source, Str2) )
    printf("\nGo Back, Destroy SERN in an another worldline");
  else
    printf("\nIf you got the references, vishwaCTF{El_Psy_Kongroo}");
  return 0;
}
```
This is basically the solution for us, we can read it using our own eyes. Let's ignore the **message** function since it doesn't really help us:
```c++
int message()
{
	puts("---------------------------------------------------");
	printf("Welcome to VishwaCTF Mini!! I hope you have fun\n");
	printf("Here's a simple problem to get you started!\n");
  return puts("---------------------------------------------------\n");
}
```
What we notice at first is the **scanf** which we see twice in the code already, once for username and once for password. For someone who didn't know what **scanf** was before you should now, it's for gathering input. 
## Username
The username is set to a variable called **Destination** and then a **checkUsername** function is called:
```c++
// From int main()
v8 = checkUsername(Destination);

// The function itself
__int64 __fastcall checkUsername(const char *a1)
{
  int v2; // [rsp+2Ch] [rbp-4h]

  v2 = strlen(a1);
  if ( v2 > 1 && v2 <= 7 )
    return 0i64;
  if ( v2 <= 7 )
    return 2i64;
  return 1i64;
}
```
The question now is.. what does this tell us?
First let's rename some variables real quick, we see that **strlen** is used to get the length of the input so we know that it does something with the length of the string (first parameter). I also set the function type from __int64 to int since it's easier for new reverse engineers to read :)
```c++
int __fastcall checkUsername(const char *param)
{
  int StrLen;
  StrLen = strlen(param); // String length
  if ( StrLen > 1 && StrLen <= 7 ) // String bigger than 1 but shorter than 8
    return 0;
  if ( StrLen <= 7 ) // Shorter than 8 characters
    return 2;
  return 1; // None of the statements match the string length, return 1
}
```
```c++
v8 = checkUsername(Destination);
```
So now we know what v8 stands for. If v8 is 0 it means that the string is longer than 1 but shorter than 8 (because of the *<= 7*). If v8 is 2 it means that it doesn't match the statement that returns 1 but is still shorter than 8 meaning that the string can be less than 1. If v8 is 1 though it means that it didn't match any of the conditions, meaning that it's longer than 7 characters.
```c++
user_status = checkUsername(Destination);
if ( user_status )                            // Above 0 (1 or 2)
{
  if ( user_status != 1 )                     // If 2 (shorter than 2)
    exit(0);                                  // Exit application (bad username)
  strcpy(Source, "@Channeler-");
  strcat(Source, Destination);
}
else                                          // user_status is 0
{
  strcpy(Source, "2.615074");
  strcat(Destination, Source);
  strcpy(Source, Destination);
}
```
So as we can see the only ***bad*** status code we can get from the **checkUsername** function is **2** since that will end up in the application closing. Well, can you blame the program? Who would want an username shorter than 2 characters.
I'll rewrite the code now so that it is easier to read for the *"noob"* brain. 
```c++
if ( user_status > 0 && user_status != 1 )
{
	strcpy(Source, "@Channeler-");
	strcat(Source, Destination);
}
else if(user_status == 1)
{
	exit(0);
}
else
{
	strcpy(Source, "2.615074");
	strcat(Destination, Source);
	strcpy(Source, Destination);
}
```
A little easier to understand don't you say? Well, now we know what type of usernames are accepted. They need to be over 1 character in length.. and that's probably it. 

## Password
Now what I decided to do was to take a quick peak at the password authentication to understand why there is so much focus on the username and weird **strcpy** and **strcat** everywhere.
```c++
printf("Type your password: ");
scanf("%30s", Str2);
if ( strcmp(Source, Str2) )
  printf("\nI'm sorry. You are not supposed to be here.");
else
  printf("\nHello, friend. You successfully cr4cked me :)");
return 0;
```
So there we see the **scanf** again, collecting input. Now the password is cached at **Str2** and later on compared to some variable named **Source**. So let's simplify the code once again.
```c++
printf("Type your password: ");
scanf("%30s", pass_input);
if ( strcmp(pass_expected, pass_input) )
  printf("\nI'm sorry. You are not supposed to be here.");
else
  printf("\nHello, friend. You successfully cr4cked me :)");
return 0;
```
I renamed **Source** to **pass_expected** and **Str2** to **pass_input** and they are exactly what they sound like.
It compares our input to the **pass_expected** and if they don't match it tells you *"I'm sorry.. blah blah blah"* but if they do match then it greets you with the *"Hello, friend.."*. 

## Username and password
Now with the variables renamed in IDA disassembler let's look at the username handling once again.
```c++
if ( user_status == 2 ) // same as: user_status > 0 && user_status != 1
{
	strcpy(pass_expected, "@Channeler-");
	strcat(pass_expected, Destination);
}
else if(user_status == 1)
{
	exit(0);
}
else
{
	strcpy(pass_expected, "2.615074");
	strcat(Destination, pass_expected);
	strcpy(pass_expected, Destination);
}
```
Let's not forget that **Destination** is our username input, I should've renamed it from the beginning but now we're going to have to live with it.

Let's first learn about **strcpy** and **strcat**.

### strcpy
Just sets the first parameter with the value of the second parameter. It's used in the program because of the variables being chars and modifying them using this function is the smoothest way possible.
### strcat
Appends a copy of the second parameter to the first parameter.

So if the status is **2** we can see that it caches the string **@Channeler-** in the variable **pass_expected** using **strcpy**. Then what it does is it uses **strcat** top put together **pass_expected** and **Destination** (which is your username).
If you would set Destination to: **"Hello"** the **pass_expected** would now look something like this:
```
@Channeler-Hello
```
But since "Hello" is below 8 characters it wouldn't jump to the "Mr." statement, but I just wanted to clarify how it works.

So now we know that if the username is longer than 7 characters the password will be: 
```
@Channeler- + UsernameInput
```

But what if the username is shorter than 8 characters? Let's jump straight into it.
```c++
strcpy(pass_expected, "@fsociety");
strcat(Destination, pass_expected);
strcpy(pass_expected, Destination);
```
For the *noob* brain I decided to write it in very basic code as if the variables were strings so that you could understand it easier.
```c++
pass_expected = "@fsociety";
Username += pass_expected;
pass_expected = Username;
```
So what it does is it adds *"@fsociety"* at the end of the username. If the username is Hello the new username would be ``Hello2.615074``.

Now that we know both algorithms we can begin cracking it.
The length of **Hello** is 5, meaning that it lads on UserStatus of **0** and that it adds 2.615074 to our username.
```
---------------------------------------------------
Welcome to VishwaCTF Mini!! I hope you have fun
Here's a simple problem to get you started!
---------------------------------------------------

Type your username: Hello
Type your password: Hello2.615074
If you got the references, vishwaCTF{El_Psy_Kongroo}
--------------------------------
```
If we enter something longer than 5 characters for example **something** it will add **@Channeler-** before our username.
```
---------------------------------------------------
Welcome to f-cr4ckme!! I hope you have fun :)
---------------------------------------------------

Type your username: something
Type your password: @Channeler-something
Hello, friend. You successfully cr4cked me :)
--------------------------------
```