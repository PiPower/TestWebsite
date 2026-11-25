const signInButton = document.getElementById("signInButton");
const registerButton = document.getElementById("registerButton");
const createAccountButton = document.getElementById("createAccountButton");
const loginDuringCreate = document.getElementById("loginDuringCreate");

signInButton.addEventListener("click", handleSignIn);
registerButton.addEventListener("click", handleRegister);
createAccountButton.addEventListener("click", handleCreateAccount);
loginDuringCreate.addEventListener("click", loginButtonDuringAccountCreate);

async function handleSignIn(event) 
{
    url = window.location.origin + "/api/login";
    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;

    try 
    {
        const response = await fetch(url, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                username,
                password
        })
        });
        const body = await response.text();
        if(response.status == 302)
        {
            const response = await fetch(window.location.origin + "/home");
            if (!response.ok) {
                throw new Error(`HTTP error: ${response.status}`);
            }

            // Wait until the entire HTML is downloaded
            const html = await response.text();
            document.open();
            document.write(html);
            document.close();
            window.history.pushState({}, '', response.url);
        }
        else if (response.status === 404 && body == "login") 
        {
            document.getElementById("username").value = "";
            document.getElementById("password").value = "";
            document.getElementById("username").placeholder  = "Username or email not found";
            document.getElementById("username").style.borderColor = "red";
            throw new Error(`Response status: ${response.status}`);
        }
    } 
    catch (error) 
    {
        console.error(error.message);
    }


}

async function handleRegister(event) 
{
    document.getElementById("login_view").style.display = "none";
    document.getElementById("create-account").style.display = "block";
}

async function handleCreateAccount(event)   
{
    url = window.location.origin + "/api/create_account";
    const create_username = document.getElementById("create-username").value;
    const create_email = document.getElementById("create-email").value;
    const create_password = document.getElementById("create-password").value;
    const repeat_password = document.getElementById("repeat-password").value;

    try 
    {
        const response = await fetch(url, {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({
                create_username,
                create_email,
                create_password,
                repeat_password
            })
        });
        const body = await response.text();
        if(response.status == 302)
        {
            window.location.href = "/home";
        }
        else if(response.status == 400)
        {
            alert(body);
        }
        
    }
    catch (error) 
    {
        console.error(error.message);
    }
}

async function loginButtonDuringAccountCreate(event)  
{
    document.getElementById("login_view").style.display = "block";
    document.getElementById("create-account").style.display = "none";
}