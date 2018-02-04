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

import java.net.URL;
import java.util.List;
import java.util.ResourceBundle;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.layout.AnchorPane;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

/**
 * FXML Controller class
 *
 * @author Matteo Chiesi - University of Bologna
 */
public class CalibrationController extends AnchorPane implements Initializable {

    public final double[] Cal = new double[64];

    @FXML
    public static Button SaveCalibration;
    
    @FXML
    private TextField CalCH1;
 
    @FXML
    public List<TextField> CalibrationList;
    
    @FXML
    public List<Label> CalibrationLabel;
    
    public CalibrationController(){
        
    }
    
    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) { 
        for(int i=0;i<64;i++){
             Cal[i] = 1.0;
         }
    }    
    
    @FXML
    private void handleSaveCalibration(ActionEvent event) throws FileNotFoundException{
         
        PrintWriter pw = new PrintWriter(new File("Calibration.csv"));
        StringBuilder sb = new StringBuilder();
        for(int i=0;i<64;i++){
             Cal[i] = Double.parseDouble(CalibrationList.get(i).getText());
             sb.append(CalibrationLabel.get(i).getText());
             sb.append(';');
             sb.append(CalibrationList.get(i).getText());
             sb.append('\n');
         }
        pw.write(sb.toString());
        pw.close();
        System.out.println("done!");  
    } 
    

     public double[] getValue(){
       return Cal;
   } 
     
     
}
