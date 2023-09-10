using System;

// Custom class 
using MicaWorkSpace;

class Program
{
    public static void Main(string[] args)
    {
        string tunnel = "http://127.0.0.1:80";
        string authkey = "tb_is_johns_son";

        MangaSee123Client client = new MangaSee123Client(tunnel, authkey);
    }
}

