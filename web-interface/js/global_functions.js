function createInitialChessGame() {


        document.querySelectorAll(".square").forEach(e => {
            e.innerHTML='';

            if(e.pos.charAt(1)=== '7'){
                e.piece='bp';
                let child= document.createElement('img');
                child.src="./images/bp.png";
                e.appendChild(child);
            }
            else if(e.pos.charAt(1) === '2'){
                e.piece='wp';

                let child= document.createElement('img');
                child.src="./images/wp.png";
                e.appendChild(child);
            }
            else if(e.pos === 'a8' || e.pos === 'h8'){
                e.piece='br';


                let child= document.createElement('img');
                child.src="./images/br.png";
                e.appendChild(child);
            }
            else if(e.pos === 'a1' || e.pos === 'h1'){
                e.piece='wr';


                let child= document.createElement('img');
                child.src="./images/wr.png";
                e.appendChild(child);
            }
            else if(e.pos === 'b8' || e.pos === 'g8'){
                e.piece='bn';

                let child= document.createElement('img');
                child.src="./images/bn.png";
                e.appendChild(child);
            }
            else if(e.pos === 'b1' || e.pos === 'g1'){
                e.piece='wn';

                let child= document.createElement('img');
                child.src="./images/wn.png";
                e.appendChild(child);
            }
            else if(e.pos === 'c8' || e.pos === 'f8'){
                e.piece='bb';

                let child= document.createElement('img');
                child.src="./images/bb.png";
                e.appendChild(child);
            }
            else if(e.pos === 'c1' || e.pos === 'f1'){
                e.piece='wb';

                let child= document.createElement('img');
                child.src="./images/wb.png";
                e.appendChild(child);
            }
            else if(e.pos === 'd8' ){
                e.piece='bq';


                let child= document.createElement('img');
                child.src="./images/bq.png";
                e.appendChild(child);
            }
            else if(e.pos === 'd1'){
                e.piece='wq';


                let child= document.createElement('img');
                child.src="./images/wq.png";
                e.appendChild(child);
            }
            else if(e.pos === 'e8' ){
                e.piece='bk';

                let child= document.createElement('img');
                child.src="./images/bk.png";
                e.appendChild(child);
            }
            else if(e.pos === 'e1'){
                e.piece='wk';
                let child= document.createElement('img');
                child.src="./images/wk.png";
                e.appendChild(child);

            }else{
                e.piece='es';

                let child= document.createElement('img');
                child.src="";
                e.appendChild(child);
            }

        });
}