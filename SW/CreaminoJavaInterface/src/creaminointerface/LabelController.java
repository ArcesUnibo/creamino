/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
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

/**
 * FXML Controller class
 *
 * @author Matteo Chiesi - University of Bologna
 */
public class LabelController extends AnchorPane implements Initializable {

    public final String[] CHLabels = new String[64];

    @FXML
    public static Button ResetLabel;
 
    //@FXML
    //public TextField LabelCH1;
    
    @FXML
    public Label CH1;
    
    @FXML
    public static List<TextField> LabelList;
    
    public LabelController(){
        
    }
    
    /**
     * Initializes the controller class.
     */
    @Override
    public void initialize(URL url, ResourceBundle rb) {
        //LabelCH1 = new TextField();
        for(int i=0;i<64;i++){
             //LabelCH1.setText(CH1.getText());
             
             //CHLabels[i] = "CH1";
         }
        
    }    
    
    @FXML
    private void handleResetButton(ActionEvent event){
         for(int i=0;i<64;i++){
             //LabelCH1.setText(CH1.getText());
             CHLabels[i] = LabelList.get(i).getText();
             //CHLabels[i] = "CH1";
         }
    } 
    

     public String[] getString(){
       return CHLabels;
   } 
     
     
}
