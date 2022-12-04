using System;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace WebApplication2.Client
{
    class Program
    {
        static async Task Main(string[] args)
        {
            Console.WriteLine("press enter to continue");
            Console.ReadLine();

            using (ClientWebSocket client = new ClientWebSocket())
            {
                Uri serviceUri = new Uri("ws://192.168.0.185/ws");
                var cTs = new CancellationTokenSource(); // cancellation token source
                cTs.CancelAfter(TimeSpan.FromSeconds(120));
                try
                {
                    await client.ConnectAsync(serviceUri, cTs.Token); // await connection, for this the main method has to be async
                    var n = 0;
                    //while (client.State == WebSocketState.Open)
                    //{
                        //receive
                        _ = ReceiveMethod(client, cTs);

                        //send 
                        _ = SendMethod(client, cTs);
                    //}
                    while (client.State == WebSocketState.Open)
                    {
                    }

                }
                catch (WebSocketException e)
                {
                    Console.WriteLine(e.Message);
                }
            }

            Console.WriteLine("Press enter to exit");
            Console.ReadLine();
        
        }

        private static async Task ReceiveMethod(ClientWebSocket client, CancellationTokenSource cTs)
        {
            var responseBuffer = new byte[1024];
            var offset = 0;
            var packageSize = 1024;
            if (client.State != WebSocketState.Open)
            {
                return;
            }
            while (true)
            {
                ArraySegment<byte> buffer = new ArraySegment<byte>(responseBuffer, offset, packageSize);
                WebSocketReceiveResult response = await client.ReceiveAsync(buffer, cTs.Token);
                var responseMessage = Encoding.UTF8.GetString(responseBuffer, offset, response.Count);
                Console.WriteLine(responseMessage);
            }
            //if (response.EndOfMessage)
            //{
            //}
        }

        private static async Task SendMethod(ClientWebSocket client, CancellationTokenSource cTs)
        {
            while (true)
            {
                Console.WriteLine("enter message to send or quit to exit");
                string message = Console.ReadLine();
                if (!string.IsNullOrEmpty(message))
                {
                    if (message.ToLower() == "quit")
                    {
                        await client.CloseAsync(WebSocketCloseStatus.NormalClosure, null, cTs.Token);
                        return;
                    }
                    ArraySegment<byte> byteToSend = new ArraySegment<byte>(Encoding.UTF8.GetBytes(message));
                    await client.SendAsync(byteToSend, WebSocketMessageType.Text, true, cTs.Token);
                    //var responseBuffer = new byte[1024];
                    //var offset = 0;
                    //var packageSize = 1024;
                    //while (true)
                    //{
                    //    ArraySegment<byte> byteReceived = new ArraySegment<byte>(responseBuffer, offset, packageSize);
                    //    WebSocketReceiveResult response = await client.ReceiveAsync(byteReceived, cTs.Token);
                    //    var responseMessage = Encoding.UTF8.GetString(responseBuffer, offset, response.Count);
                    //    Console.WriteLine(responseMessage);
                    //    if (response.EndOfMessage)
                    //    {
                    //        break;
                    //    }
                    //}
                }
            }
        }
    }
}
