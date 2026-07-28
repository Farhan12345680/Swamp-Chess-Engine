import java.io.*;


public class Cli{
    public static void printReturnString(BufferedReader reader) {
        String str;
        try{
            while((str=reader.readLine())!=null)
            {
                if(str.equals("END"))
                {
                    System.out.println();
                    break;
                }
                System.out.println(str);

            }
        }
        catch(Exception e){
            e.printStackTrace();
        }
    }
    public static void main(String[] args) throws Exception
    {
        String str;

        if(System.getProperty("os.name").toLowerCase().contains("win"))
        {
            str="../Swamp.exe";
        }
        else
        {
            str="../Swamp.out";
        }
        ProcessBuilder pb = new ProcessBuilder(str);
        Process process = pb.start();

        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(process.getOutputStream()));
        BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
        BufferedReader cliReader= new BufferedReader(new InputStreamReader(System.in));

        while(true){
            System.out.println("Swamp Chess Engine CLI");
            System.out.println("1.  Print Engine Specification");
            System.out.println("2.  Start A New Game");
            System.out.println("3.  Start A New Game From FEN");
            System.out.println("4.  Print The BitBoard of the Current Game");
            System.out.println("5.  Print White Side");
            System.out.println("6.  Print Black Side");
            System.out.println("7.  Make A Move");
            System.out.println("8.  Undo A Move");
            System.out.println("9.  Print The Game Stack");
            System.out.println("10. Print Fen String of this Position");
            System.out.println("11. Stop Engine and Exit");
            System.out.println();
            System.out.println();


           String str1 = cliReader.readLine();

           switch (str1) {
            case "1":
                writer.write("uci\n");
                writer.flush();               
                printReturnString(reader);
                break;
         
            case "2":
                writer.write("2\n");
                writer.flush();   
                printReturnString(reader);

                break;            
            case "3":
                writer.write("3\n");
                writer.flush();

                String str12 = cliReader.readLine();
                writer.write(str12+"\r\n");
                writer.flush();
                printReturnString(reader);

                break;            
            case "4":
                writer.write("4\n");
                writer.flush();  
                printReturnString(reader);
                break;
            case "5":
                break;
            case "6":
                
                break;
            case "7":
                
                break;
            case "8":
                
                break;
            case "9":
                
                break;
            case "10":
                
                break;
            default:
                break;
           }
           cliReader.read();

        }
    }
}