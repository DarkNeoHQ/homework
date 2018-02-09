#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
struct HuffNode//������������Ľṹ��
{
	unsigned char b;//ֵ
	long count;//Ƶ��
	long parent;//��
	long lch, rch;//����
	char bits[256];//��Ӧ�Ĺ���������
};
void menu();
int compress();
void reset(HuffNode HuffTreeNode[]);
void rank(HuffNode HuffTreeNode[]);
void generatTree(HuffNode HuffTreeNode[], int n, int m);
void huffmanCode(HuffNode HuffTreeNode[], int n);
int uncompress();
int main()
{
	int id = 0;
	while (1)
	{
		system("cls");
		menu();
		scanf("%d", &id);
	switch (id)
	{
	case 1:
		system("cls");
		compress();
		
		break;
	case 2:
		system("cls");
		uncompress();
		break;
	case 3:
		return 0;
		break;
	}
	}
	compress();
	uncompress();
	return 0;
}
int compress()
{
	time_t start, end;
	HuffNode HuffTreeNode[512];
	int i = 0, j = 0;
	int n = 0, m = 0;
	long min, pt, f;
	unsigned char c;
	long flength = 0;
	long length = 0;
	FILE *ifp, *ofp;
	char filename[255];
	//��ʼ��HUFFTREENODE����
	reset(HuffTreeNode);
	printf("��������Ҫѹ�� ���ļ���:\n");
	getchar();
	gets_s(filename);
	//���ļ�������Ȩ
	ifp = fopen(filename, "rb");
	start = clock();
	if (ifp == NULL)//�ж��ļ��Ƿ����
	{
		printf("\t�ļ���ʧ�ܣ�\n");
		return 0;
	}
	else
	{
		while (!feof(ifp))//����ļ�û�н�����һֱ��ȡ
		{
			fread(&c, 1, 1, ifp);//��ȡһ��
			HuffTreeNode[c].count++;//��¼Ƶ��
			flength++;//ͳ���ļ�����
		}
		flength--;
		length = flength;//ԭ�ļ����ܳ���
		HuffTreeNode[c].count;
	}
	fclose(ifp);
	//��Ҷ�ڵ��Ȩ���дӴ�С����
	rank(HuffTreeNode);
	//����Ҷ�ڵ� �ֽ��
	for (int i = 0; i < 256; i++)//Ȩֵ����
	{
		if (HuffTreeNode[i].count == 0)
			break;
		n = i + 1; //nΪҶ�ӽ�����;
		m = 2 * n - 1;
	}
	//������
	generatTree(HuffTreeNode, n, m);
	//����������
	huffmanCode(HuffTreeNode, n);
	for (i = 0; i < 35; i++)
	{
		
		printf("%d %d  %d  %d %d %d \n", i, HuffTreeNode[i].count, HuffTreeNode[i].parent, HuffTreeNode[i].lch, HuffTreeNode[i].rch,HuffTreeNode[i].bits);
		
	}
	system("pause");
	//���ļ�����ѹ��

	///////////////////////////////////////////////////////


	FILE *ifp1, *ofp1;//ifp1ָ��Դ�ļ�,ofp1ָ��ѹ������ļ�
	char filename1[255];
	ifp1 = fopen(filename, "rb");//�Զ���������ʽ���ļ���ȡ
	char buf[512];
	unsigned char z;
	buf[0] = 0;
	pt = 8, f = 0;
	j = 0, i = 0;
	printf("������ѹ������ļ���\n");
	gets_s(filename1);
	strcat(filename1, ".hub");
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
		while (j >= 8)//����8���в��,�ֳ������ֽڴ洢
		{
			for (i = 0; i<8; i++)
			{
				if (buf[i] == '1')
					z = (z << 1) | 1;
				else
					z = z << 1;
			}
			//д���ļ�ofp
			fwrite(&z, 1, 1, ofp1);
			pt++;
			strcpy(buf, buf + 8);//һ���ֽ�һ���ֽ�ȡ
			j = strlen(buf);
		}
		if (f == flength)//ȫ��ȡ�����
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
		fwrite(&z, 1, 1, ofp1);//д�����һ���ֽڽ�β
		pt++;
	}
	fseek(ofp1, 4, SEEK_SET);//�ƶ������ĸ��ֽ�
	fwrite(&pt, sizeof(long), 1, ofp1);//д��ѹ�����ļ�����,4�ֽ�
	fseek(ofp1, pt, SEEK_SET);//�ƶ���ĩβ
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
	end = clock();//ʱ�ӽ���
	fclose(ifp1);
	fclose(ofp1);
	printf("\nѹ���ļ��ɹ���\n");
	printf("ѹ����Ϊ%f%%\n\n", div * 100);
	printf("����ʱ��Ϊ:%d ����\n", end - start);
	system("pause");
}
void reset(HuffNode HuffTreeNode[])
{
	int i;
	for (i = 0; i<512; i++)
	{
		if (HuffTreeNode[i].count != 0)
		{
			HuffTreeNode[i].b = (unsigned char)i;
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
void rank(HuffNode HuffTreeNode[])
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
void generatTree(HuffNode HuffTreeNode[], int n, int m)
{
	int j, i;
	long pt, min;
	for (i = n; i<m; i++)
	{
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
		HuffTreeNode[i].count = HuffTreeNode[pt].count;
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
void huffmanCode(HuffNode HuffTreeNode[], int n)
{
	int i, j, f;
	for (i = 0; i<n; i++)
	{
		f = i;
		HuffTreeNode[i].bits[0] = 0;
		while (HuffTreeNode[f].parent != -1)
		{
			j = f;
			f = HuffTreeNode[f].parent;
			if (HuffTreeNode[f].lch == j)
			{
				j = strlen(HuffTreeNode[i].bits);
				memmove(HuffTreeNode[i].bits + 1, HuffTreeNode[i].bits, j + 1);
				HuffTreeNode[i].bits[0] = '0';
			}
			else
			{
				j = strlen(HuffTreeNode[i].bits);
				memmove(HuffTreeNode[i].bits + 1, HuffTreeNode[i].bits, j + 1);
				HuffTreeNode[i].bits[0] = '1';
			}
		}
	}
}
int uncompress()
{
	time_t start, end;
	HuffNode HuffTreeNode[512];
	char filename[255], outputfile[255], buf[255], bx[255];
	unsigned char c;
	long i, j, m, n, f, p, l;
	long flength;
	reset(HuffTreeNode);
	FILE *ifp, *ofp;
	printf(" ��������Ҫ��ѹ�����ļ���\n");
	getchar();
	gets_s(filename);
	ifp = fopen(filename, "rb");
	if (ifp == NULL)
	{
		printf("\n\t�ļ���ʧ��!\n");
		return 0;
	}
	printf("����ǰĿ¼�½�ѹ,�������ѹ����ļ�����չ��\n");
	gets_s(outputfile);
	ofp = fopen(outputfile, "wb");
	start = clock();
	if (ofp == NULL)
	{
		printf("��ѹ���ļ�ʧ��!\n");
		return 0;
	}


	fread(&flength, sizeof(long), 1, ifp);//��ȡԭ�ļ�����
	fread(&f, sizeof(long), 1, ifp);//��ȡ4���ֽڵ��ļ�����
	fseek(ifp, f, SEEK_SET);//�ƶ����洢������λ��
	fread(&n, sizeof(long), 1, ifp);//��ȡ�ڵ�����



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
			f = c;//תʮ����
			itoa(f, buf, 2);//תΪ������
			f = strlen(buf);//����8λ��0
			for (l = 8; l > f; l--)
			{
				strcat(HuffTreeNode[i].bits, "0");//���ڽ����벹0
			}
			strcat(HuffTreeNode[i].bits, buf);//�����ַ���
		}
		HuffTreeNode[i].bits[p] = 0;//������
	}
	HuffNode tmp;
	for (i=0;i<n;i++)//������
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
	p = strlen(HuffTreeNode[n - 1].bits);//�������󳤶�


	fseek(ifp, 8, SEEK_SET);//ָ��ָ���Ź����������λ��
	m = 0;
	bx[0] = 0;//�ַ���
	while (1)
	{//���ݱ��볤��,���ν���,��ԭ����λ�洢��ԭ���ֽ�
		while (strlen(bx) < (unsigned int)p)
		{
			fread(&c, 1, 1, ifp);//ȡ�ֽ�,ת��2����
			f = c;
			itoa(f, buf, 2);
			f = strlen(buf);
			for (l = 8; l > f; l--)//����0
			{
				strcat(bx, "0");
			}
			strcat(bx, buf);
		}
		for (i = 0; i < n; i++)
		{//�ұ���
			if (memcmp(HuffTreeNode[i].bits, bx, HuffTreeNode[i].count) == 0)
				break;
		}
		strcpy(bx, bx + HuffTreeNode[i].count);
		//�жϱ����Ӧ�ַ�
		c = HuffTreeNode[i].b;
		fwrite(&c, 1, 1, ofp);//д��Ŀ���ļ�
		m++;//�Ƚ�,ͳ�ƽ�ѹ����ļ�����
		if (m == flength)break;//Դ�ļ�����
	}
	fclose(ifp);
	fclose(ofp);
	end = clock();
	printf(" ��ѹ���ɹ�\n");
	if (m == flength)
	{
		printf("��ѹ�ļ���Դ�ļ���ͬ\n");
		printf("����ʱ��Ϊ:%d ��\n", (end - start) / 1000);
		system("pause");
	}
	else
	{
		printf("\t��ѹ�ļ���Դ�ļ���ͬ!\n\n");
		system("pause");
	}
	return 0;
}
void menu()
{
	printf("++++++++++++++++++++++++++++++++++++\n");
	printf("++++++++++++++++++++++++++++++++++++\n");
	printf("++++++++++++++++++++++++++++++++++++\n");
	printf("+        1:ѹ��                    +\n");
	printf("+        2:��ѹ                    +\n");
	printf("+        3:�˳�                    +\n");
	printf("++++++++++++++++++++++++++++++++++++\n");
	printf("+          by qhc                  +\n");
	printf("++++++++++++++++++++++++++++++++++++\n");
}
