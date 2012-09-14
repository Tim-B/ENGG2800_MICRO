/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication5;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.util.ArrayList;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

/**
 *
 * @author Michael
 */
class TextForm extends JPanel {
    private JTextField[] fields;
    
    public TextForm(String[] labels, char[] mnemonics, int[] widths, String[] tips) {
        super(new BorderLayout());
        JPanel labelPanel = new JPanel(new GridLayout(labels.length, 1));
        JPanel fieldPanel = new JPanel(new GridLayout(labels.length, 1));
        add(labelPanel, BorderLayout.WEST);
        add(fieldPanel, BorderLayout.CENTER);
        fields = new JTextField[labels.length];

        for (int i = 0; i < labels.length; i += 1) {
          fields[i] = new JTextField();
          if (i < tips.length)
            fields[i].setToolTipText(tips[i]);
          if (i < widths.length)
            fields[i].setColumns(widths[i]);

          JLabel lab = new JLabel(labels[i], JLabel.RIGHT);
          lab.setLabelFor(fields[i]);
          if (i < mnemonics.length)
            lab.setDisplayedMnemonic(mnemonics[i]);

          labelPanel.add(lab);
          JPanel p = new JPanel(new FlowLayout(FlowLayout.LEFT));
          p.add(fields[i]);
          fieldPanel.add(p);
        }
    }
    
    public ArrayList<Integer> getBinary(int i, int wordLength) {
        ArrayList<Integer> list = new ArrayList();
          int number;
          if(fields[i].getText().equals("")) {
              number = 0;
          } else {
              number = Integer.parseInt(fields[i].getText());
          }
          String binary = Integer.toBinaryString(number);
          System.out.println(binary);
          char charArray[] = binary.toCharArray();
          int count = wordLength - charArray.length;
          int c = 0;
          while(c < count) {
              list.add(0);
              c++;
          }
          for(char s: charArray){
              list.add(Character.getNumericValue(s));

          }
          return list;
    }
}
