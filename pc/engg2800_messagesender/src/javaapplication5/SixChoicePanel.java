/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication5;
import java.awt.*;
import javax.swing.*;

/** A JPanel that displays six JRadioButtons */

public class SixChoicePanel extends JPanel {
  public SixChoicePanel(String title, String[] buttonLabels) {
    super(new GridLayout(2, 1));
    setBackground(Color.lightGray);
    setBorder(BorderFactory.createTitledBorder(title));
    JPanel group = new JPanel();
    JTextField option;
    int halfLength = buttonLabels.length/2;  // Assumes even length
    for(int i=0; i<halfLength; i++) {
      option = new JTextField(buttonLabels[i]);
      group.add(option);
      add(option);
      option = new JTextField(buttonLabels[i+halfLength]);
      group.add(option);
      add(option);
    }
  }
}