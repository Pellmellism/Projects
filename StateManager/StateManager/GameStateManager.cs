using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace StateManager
{
    //Using Generics allows for more dynamic data serialization
    public class GameStateManager
    {
        /// <summary>
        /// We Convert the data to an byte array
        /// Return type is a byte array
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="obj"></param>
        /// <returns></returns>
        public byte[] Serialize<T>(T obj)
        {
            BinaryFormatter formatter = new BinaryFormatter();
            using (MemoryStream m = new MemoryStream())
            {
                formatter.Serialize(m, obj);
                return m.ToArray();
            }
        }
        /// <summary>
        /// We Convert bytes to the object type which is required
        /// The object type must be the same as the object type which was concerted to bytes
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="data"></param>
        /// <param name="obj"></param>
        /// <returns></returns>
        public T Desserialize<T>(byte[] data,T obj)
        {
            BinaryFormatter formatter = new BinaryFormatter();
            using (MemoryStream m = new MemoryStream(data))
            {
                obj = (T)formatter.Deserialize(m);
                return obj;
            }
            
        }

        public string result;
        //If using BinaryFormatter
        public string SaveBF<T>(string path, T state)
        {
            try
            {
                BinaryFormatter bf = new BinaryFormatter();
                FileStream saveFile = File.Create(path + "/playerInfo.bin");
                bf.Serialize(saveFile, state);
                saveFile.Close();
                saveFile.Dispose();
            }
            catch (Exception ex)
            {
                if (ex != null)
                    result = "Unsuccessful Save";
                else
                    result = "Successful Save";
                
            }
            finally
            {

            }
            return result;
        }

        public T LoadBF<T>(string path, T state)
        {
            try
            {
                if (File.Exists(path + "/playerInfo.bin"))
                {
                    BinaryFormatter bf = new BinaryFormatter();
                    FileStream loadFile = File.Open(path + "/playerInfo.bin", FileMode.Open);
                    state = (T)bf.Deserialize(loadFile);
                    loadFile.Close();
                    loadFile.Dispose();
                }
            }
            catch (Exception ex)
            {
                if (ex != null)
                    result = "Unsuccessful";
            }
            finally
            {

            }
            return state;
        }

        public string CheckError()
        {
            return result;
        }
    }

}
