import java.util.Scanner;

public class Main {
    public static void multiplication(int m_ar, int m_br) {
        double temp;

        double[] pha = new double[m_ar * m_br];
        double[] phb = new double[m_ar * m_br];
        double[] phc = new double[m_ar * m_br];

        for (int i = 0; i < m_ar; i++)
            for (int j = 0; j < m_ar; j++)
                pha[i * m_ar + j] = 1.0;

        for (int i = 0; i < m_br; i++)
            for (int j = 0; j < m_br; j++)
                phb[i * m_br + j] = (double)(i + 1);

        long timeStart = System.currentTimeMillis();

        for (int i=0;  i < m_ar; i++) {
            for (int j = 0; j < m_br; j++) {
                temp = 0;

                for (int k = 0; k < m_ar; k++)
                    temp += pha[i * m_ar + k] * phb[k * m_br + j];

                phc[i * m_ar + j] = temp;
            }
        }

        long timeEnd = System.currentTimeMillis();

        System.out.println("Elapsed Time: " + (timeEnd - timeStart) / 1000.0 + " seconds");

        System.out.println();

        System.out.println("Result Matrix: ");

        for (int i = 0; i < 1; i++)
            for (int j = 0; j < Math.min(10, m_br); j++)
                System.out.print(phc[j] + " ");

        System.out.println();
    }

    public static void lineMultiplication(int m_ar, int m_br) {

        double[] pha = new double[m_ar * m_br];
        double[] phb = new double[m_ar * m_br];
        double[] phc = new double[m_ar * m_br];

        for (int i = 0; i < m_ar; i++)
            for (int j = 0; j < m_ar; j++)
                pha[i * m_ar + j] = 1.0;

        for (int i = 0; i < m_br; i++)
            for (int j = 0; j < m_br; j++)
                phb[i * m_br + j] = (double)(i + 1);

        long timeStart = System.currentTimeMillis();

        for (int i=0;  i < m_ar; i++) {
            for (int k = 0; k < m_ar; k++) {

                for (int j = 0; j < m_br; j++)
                    phc[i * m_ar + j] += pha[i * m_ar + j] * phb[k * m_br + j];

            }
        }

        long timeEnd = System.currentTimeMillis();

        System.out.println("Elapsed Time: " + (timeEnd - timeStart) / 1000.0 + " seconds");

        System.out.println();

        System.out.println("Result Matrix: ");

        for (int i = 0; i < 1; i++)
            for (int j = 0; j < Math.min(10, m_br); j++)
                System.out.print(phc[j] + " ");

        System.out.println();
    }

    public static void main(String[] args) {
        int op;

        do {

            System.out.println();
            System.out.println("1. Multiplication");
            System.out.println("2. Line Multiplication");

            Scanner in = new Scanner(System.in);
            op = in.nextInt();

            if (op == 0)
                break;

            System.out.println("Dimensions: lines cols");

            in = new Scanner(System.in);

            String input = in.nextLine();
            String[] split = input.split("\\s+");

            if (split.length != 2) {
                System.out.println("Wrong number of arguments!");

                main(null);

                break;
            }

            int lin = Integer.parseInt(split[0]);
            int col = Integer.parseInt(split[1]);

            switch (op) {
                case 1:

                    multiplication(lin, col);

                    break;

                case 2:

                    lineMultiplication(lin, col);

                    break;

                default:
                    break;
            }

        } while (op != 0);
    }
}
