createInitialChessGame();
add_functionality();

let restart = document.querySelector("#restart");
        
restart.addEventListener('click' , createInitialChessGame); 


eval_bar_toggle.addEventListener('click' , ()=>{
    console.log('first');
    console.log(document.querySelector('.eval_holder'));
    document.querySelector('.eval_holder').classList.toggle('show');
});