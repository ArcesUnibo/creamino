/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package creaminointerface;

import java.util.concurrent.ExecutorService;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import com.aquafx_project.AquaFx;
import static javafx.application.Application.launch;
/**
 *
 * @author Matteo Chiesi - University of Bologna
 */
public class CreaminoInterface extends Application {
    
    public ExecutorService executor;
    
    @Override
    public void start(Stage stage) throws Exception {
        
        FXMLLoader fxmlLoader = new FXMLLoader(getClass().getResource("FXMLDocument.fxml"));
        Parent root = (Parent) fxmlLoader.load();
        AquaFx.style();
        
        Scene scene = new Scene(root);
        stage.setScene(scene);
        stage.setFullScreen(false);
        stage.setMaximized(false);
        scene.getStylesheets().add("newCascadeStyleSheet.css");
        
        stage.setTitle("CREAMINO INTERFACE");
        stage.show();
   
    }

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        launch(args);
    }
    
}
