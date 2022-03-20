//Stone-Bird-Water Script
const body = document.querySelector("body");
const npcchoice = document.querySelector("#npc-choice");

function NPChoice(e){
    let choiceValue = Math.random();
    if(choiceValue < 0.33)
    {
        return "Stone";
    }
    if(choiceValue < 0.66)
    {
        return "Water";
    }
    else
    {
        return "Bird";
    }
}

function logText(e)
{
    console.log(this);
}

npcchoice.addEventListener("click", NPChoice);