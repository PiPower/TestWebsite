
const homeButton = document.querySelector("#button-home button");
const exploreButton = document.querySelector("#button-explore button");
const notificationsButton = document.querySelector("#button-notifications button");
const messagesButton = document.querySelector("#button-messages button");
const bookmarksButton = document.querySelector("#button-bookmarks button");
const profileButton = document.querySelector("#button-profile button");
const postButton = document.querySelector("#button-post button");
const forYouButton = document.querySelector("#button-for-you button");
const followingButton = document.querySelector("#button-following button");
const sidebarProfile = document.querySelector(".sidebar-profile");

homeButton.addEventListener("click", homeHandler);
exploreButton.addEventListener("click", exploreHandler);
notificationsButton.addEventListener("click", notificationsHandler);
messagesButton.addEventListener("click", messagesHandler);
bookmarksButton.addEventListener("click", bookmarksHandler);
profileButton.addEventListener("click", profileHandler);
postButton.addEventListener("click", postHandler);
forYouButton.addEventListener("click", forYouHandler);
followingButton.addEventListener("click", followingHandler);
sidebarProfile.addEventListener("click", sidebarProfileHandler)

async function homeHandler() 
{
    alert("homeHandler");
}

async function exploreHandler() 
{
    alert("exploreHandler");
}

async function notificationsHandler() 
{
    alert("notificationsHandler");
}

async function messagesHandler() 
{
    alert("messagesHandler");
}

async function bookmarksHandler() 
{
    alert("bookmarksHandler");
}

async function profileHandler() 
{
    alert("profileHandler");
}

async function postHandler() 
{
    alert("postHandler");
}

async function forYouHandler() 
{
    const buttonFy = document.querySelector('#button-for-you button');
    buttonFy.style.setProperty('--visibility', 'visible');

    const buttonFol = document.querySelector('#button-following button');
    buttonFol.style.setProperty('--visibility', 'hidden');
}

async function followingHandler()
{
    const buttonFy = document.querySelector('#button-for-you button');
    buttonFy.style.setProperty('--visibility', 'hidden');

    const buttonFol = document.querySelector('#button-following button');
    buttonFol.style.setProperty('--visibility', 'visible');
}

async function sidebarProfileHandler()
{
    alert("sidebarProfileHandler");
}
