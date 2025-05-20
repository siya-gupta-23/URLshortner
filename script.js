document.getElementById("shorten-btn").addEventListener("click", async () => {
    const longUrl = document.getElementById("long-url").value.trim();
    const shortUrlDisplay = document.getElementById("short-url");
    const errorMessage = document.getElementById("error-message");
    const loadingSpinner = document.getElementById("loading-spinner");

    shortUrlDisplay.textContent = "";
    shortUrlDisplay.classList.remove("visible");
    errorMessage.textContent = "";
    errorMessage.classList.remove("visible");

    if (!longUrl) {
        Swal.fire({
            icon: 'error',
            title: 'Oops...',
            text: 'Please enter a valid URL!'
        });
        return;
    }

    try {
        loadingSpinner.style.display = "block"; // Show loading spinner

        const response = await fetch("http://localhost:8080/shorten", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ url: longUrl })
        });

        loadingSpinner.style.display = "none"; // Hide spinner when done

        const data = await response.json();

        if (response.ok && data.short_url) {
            Swal.fire({
                icon: 'success',
                title: 'URL Shortened!',
                text: 'Here is your link:',
                footer: `<a href="${data.short_url}" target="_blank">${data.short_url}</a>`
            });
            shortUrlDisplay.textContent = `Shortened URL: ${data.short_url}`;
            shortUrlDisplay.classList.add("visible");
        } else {
            throw new Error(data.error || "Something went wrong!");
        }
    } catch (error) {
        loadingSpinner.style.display = "none";
        Swal.fire({
            icon: 'error',
            title: 'Error',
            text: error.message
        });
    }
});
