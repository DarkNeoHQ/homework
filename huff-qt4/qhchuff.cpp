#include "qhchuff.h"
#include "ui_qhchuff.h"
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"time.h"
#include"windows.h"
struct HuffNode//���Ľṹ
{
        unsigned char b;
        long count;
        long parent;
        long lch, rch;
        char bits[256];
};
int value=0;
void reset(HuffNode HuffTreeNode[])//��ʼ����
{
        int i;
        for (i = 0; i<512; i++)
        {
                if (HuffTreeNode[i].count != 0)//Ȩֵ��Ϊ0
                {
                        HuffTreeNode[i].b = (unsigned char)i;//�洢�������
                }
                else
                {
                        HuffTreeNode[i].b = '\0';
                }
                HuffTreeNode[i].parent = -1;
                HuffTreeNode[i].lch = HuffTreeNode[i].rch = -1;
                HuffTreeNode[i].count = 0;
        }
}
void rank(HuffNode HuffTreeNode[])//����,��Ȩֵ�Ƚ�
{
        int i, j;
        HuffNode tmp;
        for (i = 0; i <= 256; i++)
        {
                int k = i;
                for (j = i + 1; j < 256; j++)
                {
                        if (HuffTreeNode[j].count > HuffTreeNode[k].count)
                                k = j;
                }
                if (k != i)
                {
                        tmp = HuffTreeNode[i];
                        HuffTreeNode[i] = HuffTreeNode[k];
                        HuffTreeNode[k] = tmp;
                }
        }
}
void generatTree(HuffNode HuffTreeNode[], int n, int m)//�ϲ���
{
        int j, i;
        long pt, min;
        for (i = n; i<m; i++)
        {
                min = 0x7FFFFFFF;
                for (j = 0; j<i; j++)
                {
                        if (HuffTreeNode[j].parent != -1)//�Ѿ�������,����
                                continue;
                        if (min>HuffTreeNode[j].count)
                        {
                                pt = j;
                                min = HuffTreeNode[j].count;
                                continue;
                        }
                }
                HuffTreeNode[i].count = HuffTreeNode[pt].count;//
                HuffTreeNode[pt].parent = i;
                HuffTreeNode[i].lch = pt;
                min = 0x7FFFFFFF;
                for (j = 0; j<i; j++)
                {
                        if (HuffTreeNode[j].parent != -1)
                                continue;
                        if (min>HuffTreeNode[j].count)
                        {
                                pt = j;
                                min = HuffTreeNode[j].count;
                                continue;
                        }
                }
                HuffTreeNode[i].count += HuffTreeNode[pt].count;
                HuffTreeNode[pt].parent = i;
                HuffTreeNode[i].rch = pt;
        }
}
void huffmanCode(HuffNode HuffTreeNode[], int n)//����������
{
        int i, j, f;
        for (i = 0; i<n; i++)
        {
                f = i;
                HuffTreeNode[i].bits[0] = 0;
                while (HuffTreeNode[f].parent != -1)//ȡҶ�ӽڵ�
                {
                        j = f;
                        f = HuffTreeNode[f].parent;
                        if (HuffTreeNode[f].lch == j)//�ж��븸�ڵ�Ĺ�ϵ
                        {
                                j = strlen(HuffTreeNode[i].bits);
                                memmove(HuffTreeNode[i].bits + 1, HuffTreeNode[i].bits, j + 1);//����
                                HuffTreeNode[i].bits[0] = '0';//��
                        }
                        else
                        {
                                j = strlen(HuffTreeNode[i].bits);
                                memmove(HuffTreeNode[i].bits + 1, HuffTreeNode[i].bits, j + 1);
                                HuffTreeNode[i].bits[0] = '1';//��
                        }
                }
        }
}
int Qhchuff::compress(char* filename)
{
            time_t start, end;
            HuffNode HuffTreeNode[512];
            int i = 0, j = 0;
            int n = 0, m = 0;
            long min, pt, f;
            unsigned char c;
            long flength = 0;
            long length = 0;
            FILE *ifp;
            //��ʼ��HUFFTREENODE����
            reset(HuffTreeNode);
            //printf("��������Ҫѹ�� ���ļ���:");
            //gets(filename);
            //���ļ�������Ȩ
            ifp = fopen(filename, "rb");
            start = clock();
            if (ifp == NULL)
            {
                    printf("\t�ļ���ʧ�ܣ�\n");
                    return 0;
            }
            else
            {
                    while (!feof(ifp))
                    {
                            fread(&c, 1, 1, ifp);
                            HuffTreeNode[c].count++;
                            flength++;
                    }
                    flength--;
                    length = flength;
                    HuffTreeNode[c].count--;
            }
            fclose(ifp);
            //��Ҷ�ڵ��Ȩ���дӴ�С����
            rank(HuffTreeNode);
            //����Ҷ�ڵ� �ֽ��
            for (int i = 0; i < 256; i++)
            {
                    if (HuffTreeNode[i].count == 0)
                            break;
                    n = i + 1;
                    m = 2 * n - 1;

            }
            //������
            generatTree(HuffTreeNode, n, m);
            //����������
            huffmanCode(HuffTreeNode, n);
            //���ļ�����ѹ��

            ///////////////////////////////////////////////////////


            FILE *ifp1, *ofp1;
            char filename1[255];
            strcpy(filename1,"out");
            ifp1 = fopen(filename, "rb");
            char buf[512];
            unsigned char z;
            buf[0] = 0;
            pt = 8, f = 0;
            j = 0, i = 0;
            //printf("\t������ѹ������ļ���\n");
            //gets(filename1);
            strcat(filename1, ".huf");
            ofp1 = fopen(filename1, "wb");
            fseek(ifp1, 0, SEEK_SET);
            fwrite(&flength, sizeof(int), 1, ofp1);
            fseek(ofp1, 8, SEEK_SET);
            while (!feof(ifp1))
            {
                    z = getc(ifp1);
                    f++;
                    for (i = 0; i<n; i++)
                    {
                            if (z == HuffTreeNode[i].b)
                                    break;
                    }
                    strcat(buf, HuffTreeNode[i].bits);
                    j = strlen(buf);
                    z = 0;
                    while (j >= 8)//���ڰ˽��в��,�ֳ������ֽڴ洢
                    {
                            for (i = 0; i<8; i++)
                            {
                                    if (buf[i] == '1')
                                            z = (z << 1) | 1;
                                    else
                                            z = z << 1;
                            }
                            //д���ļ�
                            fwrite(&z, 1, 1, ofp1);
                            pt++;
                            strcpy(buf, buf + 8);
                            j = strlen(buf);
                    }
                    if (f == flength)
                            break;
            }

            if (j>0)
            {
                    strcat(buf, "00000000");
                    for (i = 0; i<8; i++)
                    {
                            if (buf[i] == '1')
                                    z = (z << 1) | 1;
                            else
                                    z = z << 1;
                    }
                    fwrite(&z, 1, 1, ofp1);//д�����һ���ֽ�β��
                    pt++;
            }
            fseek(ofp1, 4, SEEK_SET);
            fwrite(&pt, sizeof(long), 1, ofp1);//д��ѹ�����ļ�����,4�ֽ�
            fseek(ofp1, pt, SEEK_SET);
            fwrite(&n, sizeof(long), 1, ofp1);//д��ڵ���
            for (i = 0; i<n; i++)
            {
                    fwrite(&(HuffTreeNode[i].b), 1, 1, ofp1);//д��ÿ����������ַ�
                    z = strlen(HuffTreeNode[i].bits);
                    fwrite(&z, 1, 1, ofp1);//д��ÿ���ַ��Ĺ��������볤��
                    j = strlen(HuffTreeNode[i].bits);//ͳ�ƹ���������ĳ���
                    if (j % 8 != 0)//����8��0
                    {
                            for (f = j % 8; f<8; f++)
                                    strcat(HuffTreeNode[i].bits, "0");
                    }
                    while (HuffTreeNode[i].bits[0] != 0)//תΪ2����
                    {
                            z = 0;
                            for (j = 0; j<8; j++)
                            {
                                    if (HuffTreeNode[i].bits[j] == '1')//ͨ��λ�����Ʒ���ʵ�ֲ�0
                                            z = (z << 1) | 1;
                                    else
                                            z = z << 1;
                            }
                            strcpy(HuffTreeNode[i].bits, HuffTreeNode[i].bits + 8);//����ת��������ַ�
                            fwrite(&z, 1, 1, ofp1);
                    }
            }
            long length00 = pt--;//ѹ������ļ���С
            double div = ((double)flength - (double)length00) / (double)flength;//����ѹ����
            end = clock();
            fclose(ifp1);
            fclose(ofp1);
            printf("\n\tѹ���ļ��ɹ���\n");
            printf("\tѹ����Ϊ%f%%\n\n", div * 100);
            printf("����ʱ��Ϊ:%d ����", end - start);
            return (end - start)/1000;
}

int Qhchuff:: uncompress(char* filename)
{

        time_t start, end;
        HuffNode HuffTreeNode[512];
        char  outputfile[255], buf[255], bx[255];
        unsigned char c;
        long i, j, m, n, f, p, l;
        long flength;
        reset(HuffTreeNode);
        FILE *ifp, *ofp;
        //printf("\t ��������Ҫ��ѹ�����ļ���");
       // gets(filename);

        ifp = fopen(filename, "rb");
        if (ifp == NULL)
        {
                printf("\n\t�ļ���ʧ��!\n");
                return 3;
        }
        printf("\t����ǰĿ¼�½�ѹ,�������ѹ����ļ�����չ��");
        //gets(outputfile);
        strcpy(outputfile,"newfile.bmp");
        ofp = fopen(outputfile, "wb");
        start = clock();
        if (ofp == NULL)
        {
                printf("\n\t ��ѹ���ļ�ʧ��!\n");
                return 4;
        }


        fread(&flength, sizeof(long), 1, ifp);//��ȡԭ�ļ�����
        fread(&f, sizeof(long), 1, ifp);//��ȡ4���ֽڵ��ļ�����
        fseek(ifp, f, SEEK_SET);//�ƶ����洢������λ��
        fread(&n, sizeof(long), 1, ifp);//��ȡ�ڵ�����


        value=20;
        ui->progressBar->setValue(value);
        for (i = 0; i < n; i++)//���¹���Ҷ�ӽڵ�
        {
                fread(&HuffTreeNode[i].b, 1, 1, ifp);//��ȡһ���ֽ�,�õ�һ�����
                fread(&c, 1, 1, ifp);//��ȡ���������볤��
                p = (long)c;
                HuffTreeNode[i].count = p;//������ı��볤��
                HuffTreeNode[i].bits[0] = 0;//��ʼ����Ϊ0
                if (p % 8 > 0)
                        m = p / 8 + 1;
                else
                        m = p / 8;
                for (j = 0; j < m; j++)
                {
                        fread(&c, 1, 1, ifp);//ȡ�������ֽ�
                        f = c;
                        itoa(f, buf, 2);//תΪ������
                        f = strlen(buf);//����8λ��0
                        for (l = 8; l > f; l--)
                        {
                                strcat(HuffTreeNode[i].bits, "0");
                        }
                        strcat(HuffTreeNode[i].bits, buf);//�����ַ���
                }
                HuffTreeNode[i].bits[p] = 0;//������
        }
        HuffNode tmp;
        value=40;
        ui->progressBar->setValue(value);
        for (i=0;i<n;i++)//������,���ݱ��볤��
        {
                for (j = i + 1; j < n; j++)
                {
                        if (strlen(HuffTreeNode[i].bits) > strlen(HuffTreeNode[j].bits))
                        {
                                tmp = HuffTreeNode[i];
                                HuffTreeNode[i] = HuffTreeNode[j];
                                HuffTreeNode[j] = tmp;
                        }
                }
        }
        p = strlen(HuffTreeNode[n - 1].bits);


        fseek(ifp, 8, SEEK_SET);//ָ��ָ���Ź����������λ��
        m = 0;
        bx[0] = 0;
        value=60;
        ui->progressBar->setValue(value);
        while (1)//���ݱ��볤��,���ν���,��ԭ����λ�洢��ԭ���ֽ�
        {
                while (strlen(bx) < (unsigned int)p)
                {
                        fread(&c, 1, 1, ifp);
                        f = c;
                        itoa(f, buf, 2);
                        f = strlen(buf);
                        for (l = 8; l > f; l--)
                        {
                                strcat(bx, "0");//����0
                        }
                        strcat(bx, buf);
                }
                for (i = 0; i < n; i++)//�ұ���
                {
                        if (memcmp(HuffTreeNode[i].bits, bx, HuffTreeNode[i].count) == 0)//�жϱ����Ӧ�ַ�
                                break;
                }
                strcpy(bx, bx + HuffTreeNode[i].count);
                c = HuffTreeNode[i].b;
                fwrite(&c, 1, 1, ofp);
                m++;
                if (m == flength)break;//�Ƚ�,ͳ�ƽ�ѹ����ļ�����
                value=100;
                ui->progressBar->setValue(value);
        }

        fclose(ifp);
        fclose(ofp);
        end = clock();
        printf("\n\t ��ѹ���ɹ�\n");
        if (m == flength)
        {
                printf("\t��ѹ�ļ���Դ�ļ���ͬ\n\n");
                printf("����ʱ��Ϊ:%d ��", (end - start) / 1000);
                int time=(end - start) / 1000;
                return 9;
        }
        else
                printf("\t��ѹ�ļ���Դ�ļ���ͬ!\n\n");
        return 0;
}

Qhchuff::Qhchuff(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Qhchuff)
{
    ui->setupUi(this);
    ui->progressBar->setOrientation(Qt::Horizontal);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
}

Qhchuff::~Qhchuff()
{


    delete ui;
}

void Qhchuff::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Qhchuff::on_pushButton_clicked()//ѹ���ļ�
{

   value=0;
   QString str=ui->textEdit->toPlainText();
   QByteArray str1=str.toLatin1();
   char* str2;
   int time;
   str2=str1.data();
   compress(str2);
   time=compress(str2);
   value=100;
   ui->progressBar->setValue(value);
   QString time1=QString::number(time,10);
   ui->textEdit->setPlainText("program use  "+time1+"  second");
   Sleep(1000);
    value=0;
    ui->progressBar->setValue(value);

}

void Qhchuff::on_pushButton_2_clicked()//��ѹ���ļ�
{
    value=0;
    QString str=ui->textEdit->toPlainText();
    QByteArray str1=str.toLatin1();
    char* str2;
    int time2;
    str2=str1.data();
    time2=uncompress(str2);
    QString time1=QString::number(time2,10);
    ui->textEdit->setPlainText("program use  "+time1+"  second");
    Sleep(1000);
     value=0;
     ui->progressBar->setValue(value);
}

