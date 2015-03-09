using System;
using System.Collections.Generic;
using System.Security;
using System.Security.Cryptography;
using System.Runtime.InteropServices;
using System.Text.RegularExpressions;
using System.IO;
using System.Text;

namespace Encrypt
{
    public class Encryption
    {
        public string EncryptFile(string inputFile, string outputFile)
        {
            string log = string.Empty;
            try
            {
                string password = @"myKey123"; // Your Key Here
                UnicodeEncoding UE = new UnicodeEncoding();
                byte[] key = UE.GetBytes(password);

                string cryptFile = outputFile;
                FileStream fsCrypt = new FileStream(cryptFile, FileMode.Create);

                RijndaelManaged RMCrypto = new RijndaelManaged();

                CryptoStream cs = new CryptoStream(fsCrypt,
                    RMCrypto.CreateEncryptor(key, key),
                    CryptoStreamMode.Write);

                FileStream fsIn = new FileStream(inputFile, FileMode.Open);

                int data;
                while ((data = fsIn.ReadByte()) != -1)
                    cs.WriteByte((byte)data);


                fsIn.Close();
                cs.Close();
                fsCrypt.Close();
                log = "Successful encrypt";
            }
            catch
            {
                log = "Encryption failed!, " + "Error";
            }
            finally
            {
                
            }
            return log;
        }

        public string DecryptFile(string inputFile, string outputFile)
        {
            string log = string.Empty;
            try
            {
                string password = @"myKey123"; // Your Key Here

                UnicodeEncoding UE = new UnicodeEncoding();
                byte[] key = UE.GetBytes(password);

                FileStream fsCrypt = new FileStream(inputFile, FileMode.Open);

                RijndaelManaged RMCrypto = new RijndaelManaged();

                CryptoStream cs = new CryptoStream(fsCrypt,
                    RMCrypto.CreateDecryptor(key, key),
                    CryptoStreamMode.Read);

                FileStream fsOut = new FileStream(outputFile, FileMode.Create);

                int data;
                while ((data = cs.ReadByte()) != -1)
                    fsOut.WriteByte((byte)data);

                fsOut.Close();
                cs.Close();
                fsCrypt.Close();
                log = "Decryption Success";
            }
            catch
            {
                log = "Decryption Error";
            }
            return log;
        }
    }
}
