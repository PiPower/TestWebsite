const signInButton = document.getElementById("signInButton");
const createAccountButton = document.getElementById("createAccountButton");

signInButton.addEventListener("click", handleSignIn);

createAccountButton.addEventListener("click", handleRegister);

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

        if (response.status === 404 && body == "login") 
        {
            document.getElementById("username").value = "";
            document.getElementById("password").value = "";
            document.getElementById("username").placeholder  = "Username or email not found";
            document.getElementById("username").style.borderColor = "red";
            throw new Error(`Response status: ${response.status}`);
        }

        const result = await response.json();
        console.log(result);
    } 
    catch (error) 
    {
        console.error(error.message);
    }


}

async function handleRegister(event) 
{
    alert("Register button clicked");
}