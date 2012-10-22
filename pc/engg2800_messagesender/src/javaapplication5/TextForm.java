
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package javaapplication5;

import java.awt.BorderLayout;
import java.awt.FlowLayout;
import java.awt.GridLayout;
import java.text.NumberFormat;
import java.util.ArrayList;
import javax.swing.JFormattedTextField;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.text.NumberFormatter;

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
        NumberFormatter nf = new NumberFormatter();
        nf.setMinimum(0);
        nf.setMaximum(23);
        NumberFormatter nf2 = new NumberFormatter();
        nf2.setMinimum(0);
        nf2.setMaximum(59);
        for (int i = 0; i < labels.length; i += 1) {
            if ((i % 2) == 0) {
                fields[i] = new JFormattedTextField(nf);
            } else {
                fields[i] = new JFormattedTextField(nf2);
            }

            if (i < tips.length) {
                fields[i].setToolTipText(tips[i]);
            }
            if (i < widths.length) {
                fields[i].setColumns(widths[i]);
            }

            JLabel lab = new JLabel(labels[i], JLabel.RIGHT);
            lab.setLabelFor(fields[i]);
            if (i < mnemonics.length) {
                lab.setDisplayedMnemonic(mnemonics[i]);
            }

            labelPanel.add(lab);
            JPanel p = new JPanel(new FlowLayout(FlowLayout.LEFT));
            p.add(fields[i]);
            fieldPanel.add(p);
        }
    }

    public ArrayList<Integer> getBinary(int hour, int minute, int wordLength) {
        ArrayList<Integer> list = new ArrayList();
        int hourInt;
        int minuteInt;
        if (fields[hour].getText().equals("")) {
            hourInt = 0;
        } else {
            hourInt = Integer.parseInt(fields[hour].getText());
        }
        if (fields[minute].getText().equals("")) {
            minuteInt = 0;
        } else {
            minuteInt = Integer.parseInt(fields[minute].getText());
        }
        int timeStamp = hourInt * 60 + minuteInt;
        System.out.println("The timestamp is " + timeStamp);
        String binary = Integer.toBinaryString(timeStamp);
        System.out.println(binary);
        char charArray[] = binary.toCharArray();
        int count = wordLength - charArray.length;
        int c = 0;
        while (c < count) {
            list.add(0);
            c++;
        }
        for (char s : charArray) {
            list.add(Character.getNumericValue(s));

        }
        return list;
    }
}
