/*
 * The MIT License
 *
 * Copyright 2017 Alma Mater Studiorum - Università di Bologna.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
