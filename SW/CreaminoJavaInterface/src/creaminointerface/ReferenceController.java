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
import javafx.scene.control.CheckBox;


/**
 *
 * @author Matteo Chiesi - University of Bologna
 */
public class ReferenceController implements Initializable {
    
    public final int[] CH = new int[64];

    @FXML
    public List<CheckBox> SelectedReference;
    
    
    public ReferenceController() {
        
    }
    
    @FXML
    private void handleCheckBoxCh(ActionEvent event){
        for(int i=0;i<64;i++)
            if(SelectedReference.get(i).isSelected())
                CH[i]=1;
            else    
                CH[i]=0;           
                
    }
    
    @FXML
    private void handleResetButton(ActionEvent event){
         for(int i=0;i<64;i++){
             SelectedReference.get(i).setSelected(false);
             CH[i]=0;
         }
    }
    
    
    @Override
    public void initialize(URL location, ResourceBundle resources) {
        for(int i=0;i<64;i++)
            CH[i]=0;
          
    }
    
   public int[] getIndex(){
       return CH;
   } 
    
    
}
