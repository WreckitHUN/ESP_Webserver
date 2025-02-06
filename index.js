const viewsInput = document.querySelector("#views");
const sendButton = document.querySelector("#send");

sendButton.addEventListener("click", () => {
  let number = Number(viewsInput.value);
  sendViews(number);
});

async function sendViews(views) {
  try {
    const response = await fetch("http://192.168.0.115:80/views", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        views,
      }),
    });
    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }
  } catch (error) {
    console.error("Error:", error);
  }
}
