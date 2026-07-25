import java.io.*;
import java.net.*;

public class Server{

    public static void main(String[] args){
        int port = 801;

        try(ServerSocket newSocket = new ServerSocket(port)){
            System.out.println("Simple HTTP server started on "+port);

            while(true)
            {
                Socket client = newSocket.accept();
                BufferedReader in  = new BufferedReader(new InputStreamReader(client.getInputStream()));

                OutputStream out = client.getOutputStream();
                
                String line;

                while((line = in.readLine() )!=null && !line.isEmpty()){
                    System.out.println(line);
                }

                String html = 
                        "<html>"
                        + "<head><title>Simple HTTP Server</title></head>"
                        + "<body>"
                        + "<h1>Hello, World!</h1>"
                        + "<p>This page is served by a Java HTTP Server.</p>"
                        + "</body></html>";

                
                String response =
                        "HTTP/1.1 200 OK\r\n" +
                        "Content-Type: text/html\r\n" +
                        "Content-Length: " + html.length() + "\r\n" +
                        "\r\n" +
                        html;

                out.write(response.getBytes());
                out.flush();

                client.close();
            }
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
    }


}