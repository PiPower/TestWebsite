const signInButton = document.getElementById("signInButton");
const createAccountButton = document.getElementById("createAccountButton");

signInButton.addEventListener("click", handleSignIn);

createAccountButton.addEventListener("click", handleRegister);

async function handleSignIn(event) 
{
    alert("Sign In button clicked");
}

async function handleRegister(event) 
{
    alert("Register button clicked");
}