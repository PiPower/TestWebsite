
"use strict";

// Update the clock every second
function updateClock() {
    document.getElementById("clock").textContent = new Date().toLocaleString();
}

setInterval(updateClock, 1000);
updateClock();

// Display the current page URL
document.getElementById("url").textContent =
    window.location.href;

// Test JavaScript execution
function testJavaScript() {
    document.getElementById("jsResult").textContent =
        "SUCCESS - JavaScript is working!";
}

// Test the server's HTTP response
async function checkServer() {
    const result = document.getElementById("serverResult");
    result.textContent = "Testing...";

    try {
        const response = await fetch(window.location.href, {
            cache: "no-store"
        });

        result.textContent =
            "HTTP " + response.status + " " +
            response.statusText;
    } catch (error) {
        result.textContent = "FAILED - " + error.message;
    }
}