const body = document.querySelector("body");

const redparagraph = document.createElement("p"); //allocating memory for paragraph element
const metoo = document.createElement("p");
const thirdheader = document.createElement("h3");
const firstheader = document.createElement("h1");
const divwrapper = document.createElement("div");

redparagraph.classList.add("paragraph"); //assigning paragraph class to <p>
redparagraph.textContent = "Hey I\"m Red!";
redparagraph.style.color = "red";

thirdheader.classList.add("thirdheader");
thirdheader.textContent = "I\"m a blue h3!";
thirdheader.style.color = "blue";

divwrapper.classList.add("divwrapper");
divwrapper.style.border = "solid 1px black";
divwrapper.style.backgroundColor = "pink";

firstheader.classList.add("firstheader");
firstheader.textContent = "I\"m in a div!";

metoo.textContent = "ME TOO!";

body.appendChild(redparagraph);
body.appendChild(thirdheader);
body.appendChild(divwrapper);

divwrapper.appendChild(firstheader);
divwrapper.appendChild(metoo);