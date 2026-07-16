        let chess_board= document.querySelector("#chess_board");
        let str = 'abcdefgh';
        let color_str= '#D3D4C0';

        for(let i = 8;i>0; i--){
            for(let j= 0 ;j<8; j++){
                let new_div = document.createElement("div");
                new_div.classList.add("square");
                new_div.style.backgroundColor=color_str;

                new_div.pos= (str.charAt(j)+i);
                new_div.piece="";
                
                chess_board.append(new_div);
                if(color_str === '#D3D4C0'){
                    color_str= '#467235';
                }else{
                    color_str= '#D3D4C0';
                }
            }

            if(color_str === '#467235'){
                color_str= '#D3D4C0';
            }else{
                color_str= '#467235';
            }
        }


        