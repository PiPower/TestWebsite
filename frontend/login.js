const signInButton = document.getElementById("signInButton");
const createAccountButton = document.getElementById("createAccountButton");

signInButton.addEventListener("click", handleSignIn);

createAccountButton.addEventListener("click", handleRegister);

async function handleSignIn(event) 
{
    url = window.location.origin + "/api/login";
    const username = document.getElementById("username").value;
    const password = document.getElementById("password").value;
    const params = new URLSearchParams({
        username: username,
        password: password });
    try 
    {
        const response = await fetch(url + "?" + params.toString() );

        if (!response.ok) 
        {
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