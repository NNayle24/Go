

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpsConfigurator;
import com.sun.net.httpserver.HttpsServer;
import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.security.KeyStore;
import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManagerFactory;

public class Web {
    private static String loadFileContent(String filePath) throws IOException {
        return new String(Files.readAllBytes(Paths.get(filePath)));
    }
    public static void main(String[] args) throws Exception {
        // Charge le stockage de clés
        char[] password = "polytech".toCharArray();  // Mot de passe pour le stockage de clés
        KeyStore ks = KeyStore.getInstance("JKS"); // JKS est un format de stockage de clés
        ks.load(Files.newInputStream(Paths.get("keystore.jks")), password); // Charge le stockage de clés

        // Set up the key manager factory
        KeyManagerFactory kmf = KeyManagerFactory.getInstance("SunX509"); // SunX509 est un algorithme de gestion de clés
        kmf.init(ks, password);

        // Set up the trust manager factory
        TrustManagerFactory tmf = TrustManagerFactory.getInstance("SunX509"); // SunX509 est un algorithme de gestion de clés
        tmf.init(ks);

        // Set up the SSL context
        SSLContext sslContext = SSLContext.getInstance("TLS");
        sslContext.init(kmf.getKeyManagers(), tmf.getTrustManagers(), new java.security.SecureRandom());

        // Create the HTTPS server
        HttpsServer server = HttpsServer.create(new InetSocketAddress(9999), 0);
        server.setHttpsConfigurator(new HttpsConfigurator(sslContext));

        // Set up the handlers for different pages
        server.createContext("/", new RootHandler());
        server.createContext("/page1", new Page1Handler());
        server.createContext("/page2", new Page2Handler());

        // Start the server
        server.setExecutor(null);
        server.start();
    }

    static class RootHandler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            String response = "Welcome to the root page!";
            exchange.sendResponseHeaders(200, response.length());
            try (OutputStream os = exchange.getResponseBody()) {
                os.write(response.getBytes());
            }
        }
    }

    static class Page1Handler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            String response = loadFileContent("file1");
            exchange.sendResponseHeaders(200, response.length());
            try (OutputStream os = exchange.getResponseBody()) {
                os.write(response.getBytes());
            }
        }
    }

    static class Page2Handler implements HttpHandler {
        @Override
        public void handle(HttpExchange exchange) throws IOException {
            String response = loadFileContent("file2");
            exchange.sendResponseHeaders(200, response.length());
            try (OutputStream os = exchange.getResponseBody()) {
                os.write(response.getBytes());
            }
        }
    }
}
