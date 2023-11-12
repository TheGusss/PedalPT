package com.example.pedalp1;

import android.content.res.AssetManager;

import org.apache.commons.net.ftp.FTP;
import org.apache.commons.net.ftp.FTPClient;
import static com.example.pedalp1.MainActivity.assetManager;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;

public class EnvioArduino {
    public void EnviarArchivo(String IPESP32, List<String> NombresArchivos) {
        FTPClient clienteftp = new FTPClient();
        try {
            // Configura la conexión y envío FTP
            clienteftp.connect(IPESP32);
            clienteftp.login("esp32prueba", "12345678");
            clienteftp.enterLocalPassiveMode();
            clienteftp.setFileType(FTP.BINARY_FILE_TYPE);

            for (String NombreArchivo : NombresArchivos) {
                // Preparando el Archivo
                InputStream inputStream = assetManager.open(NombreArchivo);

                // Crea un archivo temporal para copiar el contenido
                File outputFile = new File(MainActivity.DirectorioArchivos, "temp_file");
                FileOutputStream outputStream = new FileOutputStream(outputFile);
                byte[] buffer = new byte[1024];
                int length;
                while ((length = inputStream.read(buffer)) > 0) {
                    outputStream.write(buffer, 0, length);
                }

                FileInputStream ArchivoLocal = new FileInputStream(outputFile);

                // Se realiza el envío del archivo
                if (clienteftp.storeFile("/upload/" + NombreArchivo, ArchivoLocal)) {
                    System.out.println("Archivo " + NombreArchivo + " cargado exitosamente en el ESP32.");
                } else {
                    System.out.println("No se pudo cargar el archivo " + NombreArchivo + " en el ESP32. Motivo: " + clienteftp.getReplyString());
                }

                // Se cierran las conexiones para más seguridad
                outputStream.close();
                inputStream.close();
                ArchivoLocal.close();
            }

            clienteftp.logout();
        } catch (IOException e) {
            throw new RuntimeException(e);
        } finally {
            try {
                if (clienteftp.isConnected()) {
                    clienteftp.logout();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}