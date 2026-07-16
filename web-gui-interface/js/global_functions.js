function createInitialChessGame() {
        document.querySelectorAll(".square").forEach(e => {
            e.innerHTML='';

            if(e.pos.charAt(1)=== '7'){
                e.piece='bp';
                let child= document.createElement('img');
                child.src="./images/bp.png";
                child.draggable=true;
                e.appendChild(child);
            }
            else if(e.pos.charAt(1) === '2'){
                e.piece='wp';

                let child= document.createElement('img');
                child.src="./images/wp.png";
                child.draggable=true;
                e.appendChild(child);
            }
            else if(e.pos === 'a8' || e.pos === 'h8'){
                e.piece='br';


                let child= document.createElement('img');
                child.src="./images/br.png";
                child.draggable=true;

                e.appendChild(child);
            }
            else if(e.pos === 'a1' || e.pos === 'h1'){
                e.piece='wr';


                let child= document.createElement('img');
                child.src="./images/wr.png";
                child.draggable=true;
                e.appendChild(child);
            }
            else if(e.pos === 'b8' || e.pos === 'g8'){
                e.piece='bn';

                let child= document.createElement('img');
                child.src="./images/bn.png";
                child.draggable=true;
                e.appendChild(child);
            }
            else if(e.pos === 'b1' || e.pos === 'g1'){
                e.piece='wn';

                let child= document.createElement('img');
                child.src="./images/wn.png";
                child.draggable=true;

                e.appendChild(child);
            }
            else if(e.pos === 'c8' || e.pos === 'f8'){
                e.piece='bb';

                let child= document.createElement('img');
                child.src="./images/bb.png";
                child.draggable=true;

                e.appendChild(child);
            }
            else if(e.pos === 'c1' || e.pos === 'f1'){
                e.piece='wb';

                let child= document.createElement('img');
                child.src="./images/wb.png";
                child.draggable=true;

                e.appendChild(child);
            }
            else if(e.pos === 'd8' ){
                e.piece='bq';


                let child= document.createElement('img');
                child.src="./images/bq.png";
                child.draggable=true;

                e.appendChild(child);
            }
            else if(e.pos === 'd1'){
                e.piece='wq';


                let child= document.createElement('img');
                child.src="./images/wq.png";
                child.draggable=true;

                e.appendChild(child);
            }
            else if(e.pos === 'e8' ){
                e.piece='bk';

                let child= document.createElement('img');
                child.src="./images/bk.png";
                child.draggable=true;

                e.appendChild(child);
            }
            else if(e.pos === 'e1'){
                e.piece='wk';
                let child= document.createElement('img');
                child.src="./images/wk.png";
                child.draggable=true;

                e.appendChild(child);

            }else{
                e.piece='es';
            }
        });
}



let piece=null;

function dragStart(e){
    piece= e.target;            
}

function dragOver(e){
    e.preventDefault();
}


function drop(e){
    e.preventDefault();

    const parent = piece.parentNode;

    
    e.currentTarget.innerHTML = '';
    e.currentTarget.appendChild(piece);
    e.currentTarget.piece = parent.piece; 
    parent.piece='es';
    

}

function add_functionality(){
    document.querySelectorAll("img").forEach(e =>{
        e.addEventListener('dragstart', dragStart);
    });

    document.querySelectorAll('.square').forEach(e => {
        e.addEventListener('dragover' , dragOver);
        e.addEventListener('drop', drop);
    });

}



