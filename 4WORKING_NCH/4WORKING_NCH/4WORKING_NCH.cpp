// 4WORKING_NCH.cpp: определяет точку входа для консольного приложения.
//

#include <stdio.h>
#include <"ldl.h">
#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <math.h>
#include <fstream>


using namespace std;
using std::string;
using std::cout;
using std::cin;



#define N 3 /* A is 10-by-10 */
#define ANZ 5 /* # of nonzeros on diagonal and upper triangular part of A */
#define LNZ 13 /* # of nonzeros below the diagonal of L */

double FinalAX [ANZ];
int FinalIA[N+1];
int FinalJa[ANZ];
double FinalD[N];



//=================================================================================================

void ldl_symbolic
(
	int n, /* A and L are n-by-n, where n >= 0 */
	int Ap[], /* input of size n+1, not modified */
	int Ai[], /* input of size nz=Ap[n], not modified */
	int Lp[], /* output of size n+1, not defined on input */
	int Parent[], /* output of size n, not defined on input */
	int Lnz[], /* output of size n, not defined on input */
	int Flag[], /* workspace of size n, not defn. on input or output */
	int P[], /* optional input of size n */
	int Pinv[] /* optional output of size n (used if P is not NULL) */
)
{
	int i, k, p, kk, p2;
	if (P)
	{
		/* If P is present then compute Pinv, the inverse of P */
		for (k = 0; k < n; k++)
		{
			Pinv[P[k]] = k;
		}
	}



	for (k = 0; k < n; k++)
	{
		/* L(k,:) pattern: all nodes reachable in etree from nz in A(0:k-1,k) */
		Parent[k] = -1; /* parent of k is not yet known */
		Flag[k] = k; /* mark node k as visited */
		Lnz[k] = 0; /* count of nonzeros in column k of L */
		kk = (P) ? (P[k]) : (k); /* kth original, or permuted, column */
		p2 = Ap[kk + 1];
		for (p = Ap[kk]; p < p2; p++)
		{
			/* A (i,k) is nonzero (original or permuted A) */
			i = (Pinv) ? (Pinv[Ai[p]]) : (Ai[p]);
			if (i < k)
			{
				/* follow path from i to root of etree, stop at flagged node */
				for (; Flag[i] != k; i = Parent[i])
				{
					/* find parent of i if not yet determined */
					if (Parent[i] == -1) Parent[i] = k;
					Lnz[i]++; /* L (k,i) is nonzero */
					Flag[i] = k; /* mark i as visited */
				}
			}
		}
	}



	/* construct Lp index array from Lnz column counts */
	Lp[0] = 0;
	for (k = 0; k < n; k++)
	{
		Lp[k + 1] = Lp[k] + Lnz[k];
	}
}

//**********************************************************
void ldl_symbolic_5 // FINAAAL !!!!     IDEA - CHANGE BOUNDS OF CYCLES
(
	int n, /* A and L are n-by-n, where n >= 0 */
	int Ap[], /* input of size n+1, not modified */
	int Ai[], /* input of size nz=Ap[n], not modified */
	int Lp[], /* output of size n+1, not defined on input */
	int Parent[], /* output of size n, not defined on input */
	int Lnz[], /* output of size n, not defined on input */
	int Flag[], /* workspace of size n, not defn. on input or output */
	int P[], /* optional input of size n */
	int Pinv[] /* optional output of size n (used if P is not NULL) */
)
{
	int k, p, i, kk, p2;
	if (P)
	{
		/* If P is present then compute Pinv, the inverse of P */
		for (p = 0; p < n; p++)
		{
			Pinv[P[p]] = p;
		}
	}
	// common block


	for (p = 1; p <= n; p++)
	{
		/* L(k,:) pattern: all nodes reachable in etree from nz in A(0:k-1,k) */
		Parent[p - 1] = -1; /* parent of k is not yet known */
		Flag[p - 1] = p - 1; /* mark node k as visited */
		Lnz[p - 1] = 0; /* count of nonzeros in column k of L */
		kk = (P) ? (P[p - 1]) : (p - 1); /* kth original, or permuted, column */
		p2 = Ap[kk + 1];

		for (i = Ap[kk] - 1; i < p2 - 1; i++)
		{
			/* A (i,k) is nonzero (original or permuted A) */
			k = (Pinv) ? (Pinv[Ai[i + 1]]) : (Ai[i + 1]);
			if (k < p - 1)
			{
				/* follow path from i to root of etree, stop at flagged node */
				for (; Flag[k] != (p - 1); k = Parent[k])
				{
					/* find parent of i if not yet determined */
					if (Parent[k] == -1) Parent[k] = p - 1;
					Lnz[k]++; /* L (k,i) is nonzero */
					Flag[k] = p - 1; /* mark i as visited */
				}
			}
		}


	}


	// common block
	/* construct Lp index array from Lnz column counts */
	Lp[0] = 0;
	for (p = 0; p < n; p++)
	{
		Lp[p + 1] = Lp[p] + Lnz[p];
	}
}
//*********************************************************************************
void ldl_symbolic_6 // FINAAAL !!!! IDEA  - SPLIT  WORK WITH MATRICS
(
	int n, /* A and L are n-by-n, where n >= 0 */
	int Ap[], /* input of size n+1, not modified */
	int Ai[], /* input of size nz=Ap[n], not modified */
	int Lp[], /* output of size n+1, not defined on input */
	int Parent[], /* output of size n, not defined on input */
	int Lnz[], /* output of size n, not defined on input */
	int Flag[], /* workspace of size n, not defn. on input or output */
	int P[], /* optional input of size n */
	int Pinv[] /* optional output of size n (used if P is not NULL) */
)
{
	int k, p, i, kk, p2;
	if (P)
	{
		/* If P is present then compute Pinv, the inverse of P */
		for (p = 0; p < n; p++)
		{
			Pinv[P[p]] = p;
		}
	}
	// common block


	for (p = 1; p <= n; p++)
	{
		/* L(k,:) pattern: all nodes reachable in etree from nz in A(0:k-1,k) */
		Parent[p - 1] = -1; /* parent of k is not yet known */
		Flag[p - 1] = p - 1; /* mark node k as visited */
		Lnz[p - 1] = 0; /* count of nonzeros in column k of L */
		kk = (P) ? (P[p - 1]) : (p - 1); /* kth original, or permuted, column */
		p2 = Ap[kk + 1];

		for (i = Ap[kk] - 1; i < p2 - 1; i++)
		{
			/* A (i,k) is nonzero (original or permuted A) */
			k = (Pinv) ? (Pinv[Ai[i + 1]]) : (Ai[i + 1]);
			if (k < p - 1)
			{
				/* follow path from i to root of etree, stop at flagged node */
				for (; Flag[k] != (p - 1); k = Parent[k])
				{
					/* find parent of i if not yet determined */
					if (Parent[k] == -1) Parent[k] = p - 1;
					Lnz[k]++; /* L (k,i) is nonzero */   // ONLY LNZ
				}
			}
		}

		for (i = Ap[kk] - 1; i < p2 - 1; i++)
		{
			/* A (i,k) is nonzero (original or permuted A) */
			k = (Pinv) ? (Pinv[Ai[i + 1]]) : (Ai[i + 1]);
			if (k < p - 1)
			{
				/* follow path from i to root of etree, stop at flagged node */
				for (; Flag[k] != (p - 1); k = Parent[k])
				{
					/* find parent of i if not yet determined */
					if (Parent[k] == -1) Parent[k] = p - 1;
					Flag[k] = p - 1; /* mark i as visited */   //ONLY FLAG
				}
			}
		}
		


	}


	// common block
	/* construct Lp index array from Lnz column counts */
	Lp[0] = 0;
	for (p = 0; p < n; p++)
	{
		Lp[p + 1] = Lp[p] + Lnz[p];
	}
}
//*********************************************************************************

int ldl_numeric /* returns n if successful, k if D (k,k) is zero */
(
	int n, /* A and L are n-by-n, where n >= 0 */
	int Ap[], /* input of size n+1, not modified */
	int Ai[], /* input of size nz=Ap[n], not modified */
	double Ax[], /* input of size nz=Ap[n], not modified */
	int Lp[], /* input of size n+1, not modified */
	int Parent[], /* input of size n, not modified */
	int Lnz[], /* output of size n, not defn. on input */
	int Li[], /* output of size lnz=Lp[n], not defined on input */
	double Lx[], /* output of size lnz=Lp[n], not defined on input */
	double D[], /* output of size n, not defined on input */
	double Y[], /* workspace of size n, not defn. on input or output */
	int Pattern[], /* workspace of size n, not defn. on input or output */
	int Flag[], /* workspace of size n, not defn. on input or output */
	int P[], /* optional input of size n */
	int Pinv[] /* optional input of size n */
)
{

	double yi, l_ki;
	int i, k, p, kk, p2, len, top;
	for (k = 0; k < n; k++)
	{
		/* compute nonzero Pattern of kth row of L, in topological order */
		Y[k] = 0.0; /* Y(0:k) is now all zero */
		top = n; /* stack for pattern is empty */
		Flag[k] = k; /* mark node k as visited */
		Lnz[k] = 0; /* count of nonzeros in column k of L */
		kk = (P) ? (P[k]) : (k); /* kth original, or permuted, column */
		p2 = Ap[kk + 1];

		for (p = Ap[kk]; p < p2; p++)
		{
			i = (Pinv) ? (Pinv[Ai[p]]) : (Ai[p]); /* get A(i,k) */
			if (i <= k)
			{
				Y[i] += Ax[p]; /* scatter A(i,k) into Y (sum duplicates) */
				for (len = 0; Flag[i] != k; i = Parent[i])
				{
					Pattern[len++] = i; /* L(k,i) is nonzero */
					Flag[i] = k; /* mark i as visited */
				}
				while (len > 0) Pattern[--top] = Pattern[--len];
			}
		}



		/* compute numerical values kth row of L (a sparse triangular solve) */
		D[k] = Y[k]; /* get D(k,k) and clear Y(k) */
		Y[k] = 0.0;
		for (; top < n; top++)
		{
			i = Pattern[top]; /* Pattern [top:n-1] is pattern of L(:,k) */
			yi = Y[i]; /* get and clear Y(i) */
			Y[i] = 0.0;
			p2 = Lp[i] + Lnz[i];
			for (p = Lp[i]; p < p2; p++)
			{
				Y[Li[p]] -= Lx[p] * yi;
			}
			l_ki = yi / D[i]; /* the nonzero entry L(k,i) */
			D[k] -= l_ki * yi;
			Li[p] = k; /* store L(k,i) in column form of L */
			Lx[p] = l_ki;
			Lnz[i]++; /* increment count of nonzeros in col i */
		}
		if (D[k] == 0.0) return (k); /* failure, D(k,k) is zero */
	}
	return (n); /* success, diagonal of D is all nonzero */
}

//==================================================================================================
int ldl_numeric_2 /* returns n if successful, k if D (k,k) is zero */
(
	int n, /* A and L are n-by-n, where n >= 0 */
	int Ap[], /* input of size n+1, not modified */
	int Ai[], /* input of size nz=Ap[n], not modified */
	double Ax[], /* input of size nz=Ap[n], not modified */
	int Lp[], /* input of size n+1, not modified */
	int Parent[], /* input of size n, not modified */
	int Lnz[], /* output of size n, not defn. on input */
	int Li[], /* output of size lnz=Lp[n], not defined on input */
	double Lx[], /* output of size lnz=Lp[n], not defined on input */
	double D[], /* output of size n, not defined on input */
	double Y[], /* workspace of size n, not defn. on input or output */
	int Pattern[], /* workspace of size n, not defn. on input or output */
	int Flag[], /* workspace of size n, not defn. on input or output */
	int P[], /* optional input of size n */
	int Pinv[] /* optional input of size n */
)
{

	double yi, l_ki;
	int i, k, p, kk, p2, len, top;
	for (k = 0; k < n; k++)
	{
		/* compute nonzero Pattern of kth row of L, in topological order */
		Y[k] = 0.0; /* Y(0:k) is now all zero */
		top = n; /* stack for pattern is empty */
		Flag[k] = k; /* mark node k as visited */
		Lnz[k] = 0; /* count of nonzeros in column k of L */
		kk = (P) ? (P[k]) : (k); /* kth original, or permuted, column */
		p2 = Ap[kk + 1];

		for (p = Ap[kk]; p < p2; p++)
		{
			i = (Pinv) ? (Pinv[Ai[p]]) : (Ai[p]); /* get A(i,k) */
			if (i <= k)
			{
				Y[i] += Ax[p]; /* scatter A(i,k) into Y (sum duplicates) */
				for (len = 0; Flag[i] != k; i = Parent[i])
				{
					Pattern[len++] = i; /* L(k,i) is nonzero */
					Flag[i] = k; /* mark i as visited */
				}
				while (len > 0) Pattern[--top] = Pattern[--len];
			}
		}



		/* compute numerical values kth row of L (a sparse triangular solve) */
		D[k] = Y[k]; /* get D(k,k) and clear Y(k) */
		Y[k] = 0.0;
		for (; top < n; top++)
		{
			i = Pattern[top]; /* Pattern [top:n-1] is pattern of L(:,k) */
			yi = Y[i]; /* get and clear Y(i) */
			Y[i] = 0.0;
			p2 = Lp[i] + Lnz[i];
			for (p = Lp[i]; p < p2; p++)
			{
				Y[Li[p]] -= Lx[p] * yi;
			}
			l_ki = yi / D[i]; /* the nonzero entry L(k,i) */
			D[k] -= l_ki * yi;
			Li[p] = k; /* store L(k,i) in column form of L */
			Lx[p] = l_ki;
			Lnz[i]++; /* increment count of nonzeros in col i */
		}
		if (D[k] == 0.0) return (k); /* failure, D(k,k) is zero */
	}
	return (n); /* success, diagonal of D is all nonzero */
}








//==================================================================================================


void ldl_lsolve
(
	int n, /* L is n-by-n, where n >= 0 */
	double X[], /* size n. right-hand-side on input, soln. on output */
	int Lp[], /* input of size n+1, not modified */
	int Li[], /* input of size lnz=Lp[n], not modified */
	double Lx[] /* input of size lnz=Lp[n], not modified */
)
{
	int j, p, p2;
	for (j = 0; j < n; j++)
	{
		p2 = Lp[j + 1];
		for (p = Lp[j]; p < p2; p++)
		{
			X[Li[p]] -= Lx[p] * X[j];
		}
	}
}


void ldl_dsolve
(
	int n, /* D is n-by-n, where n >= 0 */
	double X[], /* size n. right-hand-side on input, soln. on output */
	double D[] /* input of size n, not modified */
)
{
	int j;
	for (j = 0; j < n; j++)
	{
		X[j] /= D[j];
	}
}


void ldl_ltsolve
(
	int n, /* L is n-by-n, where n >= 0 */
	double X[], /* size n. right-hand-side on input, soln. on output */
	int Lp[], /* input of size n+1, not modified */
	int Li[], /* input of size lnz=Lp[n], not modified */
	double Lx[] /* input of size lnz=Lp[n], not modified */
)
{
	int j, p, p2;
	for (j = n - 1; j >= 0; j--)
	{
		p2 = Lp[j + 1];
		for (p = Lp[j]; p < p2; p++)
		{
			X[j] -= Lx[p] * X[Li[p]];
		}
	}
}



//*************************************************************************************************






void convertMatrixToARR ( double **FinalMatr)
{

	int IA_out[N + 1], JA_out[ANZ];
	double Ax_out[ANZ];
	int nCHZ = 0;
	int nIA_CH = 0;

	IA_out[0] = 0;

	for (int i = 0; i < N; i++)
	{

		for (int j = 0;j < N; j++)
		{

			if ((FinalMatr[j][i] != 0) && (j <= i))
			{
				Ax_out[nCHZ] = FinalMatr[j][i];
				JA_out[nCHZ] = j;
				nCHZ++;
				nIA_CH++;
			}

		}
		IA_out[i + 1] = nIA_CH;
	}

	for (int i = 0; i < ANZ; i++) {
		FinalAX[i] = Ax_out[i];
	}
	for (int i = 0; i < N + 1; i++) {
		FinalIA[i] = IA_out[i];
	}
	for (int i = 0; i < ANZ; i++) {
		FinalJa[i] = JA_out[i];
	}

	cout << "Final edition Ax :  " << "\n";
	for (int i = 0; i< ANZ; i++)
	{
		cout << Ax_out[i] << "  ";
		if (i == 9) cout << "\n";
	}
	cout << "\n\n";

	cout << "Final edition JA :  " << "\n";
	for (int i = 0; i< ANZ; i++)
	{
		cout << JA_out[i] << "  ";
		if (i == 9) cout << "\n";
	}
	cout << "\n\n";


	cout << "Final edition IA :  " << "\n";
	for (int i = 0; i< N + 1; i++)
	{
		cout << IA_out[i] << "  ";
	}

	// ------------------------разряж----------------------------------

	// разряженный построковый формат
	cout << "\n--------------------------------\n\n LINEAR Postr Format";

	int LinearIA[N + 1];
	int LinearJA[ANZ];
	double LinearAX[ANZ];
	int nForAx = 0;
	int nChStr = 0;
	LinearIA[0] = 1;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if ((FinalMatr[i][j] != 0) && (j >= i))
			{
				LinearAX[nForAx] = FinalMatr[i][j];
				LinearJA[nForAx] = 1 + j;
				nChStr++;
				nForAx++;

			}

		}
		LinearIA[i + 1] = nChStr;
	}
	LinearIA[N] = (nChStr + 1);

	cout << "\n\nlinear IA:   " ;
	for (int i = 0; i < N + 1; i++)
	{
		cout << LinearIA[i] << " ";
	}

	cout << "\n" << "linear JA:   " ;
	for (int i = 0; i < ANZ; i++)
	{
		cout << LinearJA[i] << " ";
	}

	cout << "\n" << "linear Ax:   " ;
	for (int i = 0; i < ANZ; i++)
	{
		cout << LinearAX[i] << " ";
	}

	cout << "\n\n";

	//******************************************


}



int main(int argc, int **argv)

{
	/*int Ap[N + 1] = { 0, 1, 2, 3, 4, 6, 7, 9, 11, 15, ANZ },

		Ai[ANZ] = { 0, 1, 2, 3, 1,4, 5, 4,6, 4,7, 0,4,7,8, 1,4,6,9 };


	double Ax[ANZ] = { 1.7, 1., 1.5, 1.1, .02,2.6, 1.2, .16,1.3, .09,1.6,
		.13,.52,.11,1.4, .01,.53,.56,3.1 },

		b[N] = { .287, .22, .45, .44, 2.486, .72, 1.55, 1.424, 1.621, 3.759 };

	int Ap2[N + 1] = { 1,3,6,7,8,13,14,16,18,19,20 },
		Ai2[ANZ] = { 1,9,2,5,10,3,4,5,7,8,9,10,6,7,10,8,9,9,10 };
	double Ax2[ANZ] = { 1.7, .13, 1.,.02,.1,1.5,1.1,2.6,.16,.09,.52, .53, 1.2,1.3,.56,1.6,.11,1.4,3.1 };
*/

	//changeView(Ap2,11, Ai2, Ax2);

	
	//-----------------------------------------работа с матрицей из файла----------------------------
	int i = 0;
	int j = 0;
	double a[N + 1][N + 1];
	ifstream f("InputTestMatr.txt");


	while (!f.eof())
	{
		if (j == N + 1)
		{
			i++;
			j = 0;
			cout << endl;
		}
		f >> a[i][j];
		cout << a[i][j] << " ";
		j++;
	}
	cout << "\n\nit is all input from file\n----------------------------------\n";


	for (int i =0 ; i < N; i++) {
		FinalD[i] = a[i][N];
		cout << FinalD[i] << "\n";
	}
	cout << "\nB matrix\n----------------------------------------\n";

	double ** Matr = new double *[N];
	for (int i = 0; i < N; i++) {
	Matr[i] = new double[N];
	}

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (a[i][j] != 0)
			{
				Matr[i][j] = a[i][j];
			}
			else
			{
				Matr[i][j] = 0;
			}
			cout << Matr[i][j] << " ";
		}
		cout << "\n";
	}

	cout << "\n" << "*******************************************" << "\n";
	convertMatrixToARR(Matr);

	//-----------------------------------------------------завершили -----------------------------


	double Lx[LNZ], D[N], Y[N];
	int Li[LNZ], Lp[N + 1], Parent[N], Lnz[N], Flag[N], Pattern[N], d, k;
	/* factorize A into LDL’ (P and Pinv not used) */

	ldl_symbolic(N, FinalIA, FinalJa, Lp, Parent, Lnz, Flag, NULL, NULL);
	printf("Nonzeros in L, excluding diagonal: %d\n", Lp[N]);

	d = ldl_numeric(N, FinalIA, FinalJa, FinalAX, Lp, Parent, Lnz, Li, Lx, D, Y, Pattern,
														Flag, NULL, NULL);

	//--------------------------------общая часть-------------------------------------------------

	if (d == N)
	{
		/* solve Ax=b, overwriting b with the solution x */
		ldl_lsolve(N, FinalD, Lp, Li, Lx);
		ldl_dsolve(N, FinalD, D);
		ldl_ltsolve(N, FinalD, Lp, Li, Lx);
		for (k = 0; k < N; k++) printf("x [%d] = %g\n", k, FinalD[k]);
	}
	else
	{
		printf("ldl_numeric failed, D (%d,%d) is zero\n", d, d);
	}

	cout << "FINISH !";
	int ar;
	cin >> ar;
	return (0);
}











void changeView(int IA[], int iaLength, int JA[], double Ax[])
{
	// ================================ Write out all input arrays  ==================================
	cout << "Input IA" << "\n";
	for (int i = 0; i < iaLength; i++)
	{
		cout << IA[i] << "  ";
	}

	cout << "\n\n";
	cout << "Input JA" << "\n";
	for (int i = 0; i < ANZ; i++)
	{
		cout << JA[i] << "  ";
	}
	cout << "\n\n";
	cout << "Input Ax" << "\n";
	for (int i = 0; i < ANZ; i++)
	{
		cout << Ax[i] << "  ";
	}
	cout << "\n\n\n";
	// ================================ End of writing out all input mass  ======

	double FinalMatr[N][N];
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++) {
			FinalMatr[i][j] = 0;
		}
	}

	//=============================== Write out matrix =========================
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << fixed << FinalMatr[i][j];
			cout << "  ";
		}
		cout << "\n";
	}
	cout << " \n\n";
	//======================================================================


	int nCh = 0;
	int nDelta = 0;
	cout << " delta of IA  : ";
	for (int i = 0;i < N; i++)
	{
		nDelta = (IA[i + 1] - IA[i]);
		cout << nDelta << "  ";
	}



	for (int i = 0;i < N; i++)
	{
		nDelta = (IA[i + 1] - IA[i]);// amount of elems in row
		if (nDelta != 0)
		{
			FinalMatr[i][JA[nCh] - 1] = Ax[nCh];
			FinalMatr[JA[nCh] - 1][i] = Ax[nCh];
			nCh++;
			nDelta -= 1;

			while (nDelta != 0)
			{
				FinalMatr[i][JA[nCh] - 1] = Ax[nCh];
				FinalMatr[JA[nCh] - 1][i] = Ax[nCh];
				nCh++;
				nDelta -= 1;
			}
		}
	}


	cout << "\n\n";
	//=============================== Write out matrix ======
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << fixed << FinalMatr[i][j];
			cout << "  ";
		}
		cout << "\n";
	}
	cout << " \n\n";
	//==================================================

	int IA_out[N + 1], JA_out[ANZ];
	double Ax_out[ANZ];
	int nCHZ = 0;
	int nIA_CH = 0;

	IA_out[0] = 0;

	for (int i = 0; i < N; i++)
	{

		for (int j = 0;j < N; j++)
		{

			if ((FinalMatr[j][i] != 0) && (j <= i))
			{
				Ax_out[nCHZ] = FinalMatr[j][i];
				JA_out[nCHZ] = j;
				nCHZ++;
				nIA_CH++;
			}

		}
		IA_out[i + 1] = nIA_CH;
	}

	cout << "Final edition Ax :  " << "\n";
	for (int i = 0; i< ANZ; i++)
	{
		cout << Ax_out[i] << "  ";
		if (i == 9) cout << "\n";
	}
	cout << "\n\n\n";

	cout << "Final edition JA :  " << "\n";
	for (int i = 0; i< ANZ; i++)
	{
		cout << JA_out[i] << "  ";
		if (i == 9) cout << "\n";
	}
	cout << "\n\n\n";


	cout << "Final edition IA :  " << "\n";
	for (int i = 0; i< N + 1; i++)
	{
		cout << IA_out[i] << "  ";
	}
}







//	double FinalMatr[N][N];
//	for (int i = 0; i < N; i++)
//	{
//		for (int j = 0; j < N; j++) {
//			FinalMatr[i][j] = 0;
//		}
//		cout << "\n";
//	}

//	int nVSTch = 0;
//	/*for (int i = 0; i < N; i++)
//	{
//		for (int j = 0; j < N; j++)
//		{
//			if (Ap[i]!=Ap[i+1])
//			{
//				nVSTch = Ap[i + 1] - Ap[i]-1;
//					for (int k = 0; k < nVSTch; k++) 
//					{
//						FinalMatr[Ai[i]][j] = Ax[Ap[i + 1]];
//					}
//			}
//		}
//	}*/
//	int n_sch = 0;
//
//	for ( int i=0; i<N; i++)
//	{
//		if (Ap[i+2] != (Ap[i + 1]+1) )
//		{
//			nVSTch = Ap[i + 2] - Ap[i+1] - 1;
//			n_sch = i;
//			for (int k = 0; k <= nVSTch; k++)
//			{
//				FinalMatr[Ai[n_sch]][i] = Ax[n_sch + 1];
//				n_sch++;
//			}
//		}
//	}
//
//
//
//	for (int i = 0; i < N;i++)
//	{
//		FinalMatr[i][i] = Ax[Ap[i + 1] - 1];
//	}
//
//	// Ap[N + 1];
//	/*int n = 0;
//	for (int i = 0; i < N; i++) 
//	{
//
//		for (int j = 0; j < ANZ; j++)
//		{
//			if (i == Ai[j]) 
//			{
//			//
//				FinalMatr [Ai[j]] [i] = Ax[Ap[i+1]];
//			}
//		}
//	}*/
//
//
//	//======
//	for (int i = 0; i < N; i++) {
//		for (int j = 0; j < N; j++) {
//			cout << fixed<< FinalMatr[i][j];
//			cout << "  ";
//		}
//		cout << "\n";
//	}
//	//=======
//










//	//********************************************************************************
//
//
//	//double Lx[LNZ], D[N], Y[N];
//	//int Li[LNZ], Lp[N + 1], Parent[N], Lnz[N], Flag[N], Pattern[N], d, i;
//
//	//// factorize A into LDL’ (P and Pinv not used) 
//	////**********************************************************************
//	//ldl_symbolic(N, Ap, Ai, Lp, Parent, Lnz, Flag, NULL, NULL);
//	//printf("Nonzeros in L, excluding diagonal: %d\n", Lp[N]);
//	//d = ldl_numeric(N, Ap, Ai, Ax, Lp, Parent, Lnz, Li, Lx, D, Y, Pattern,Flag, NULL, NULL);
//
//
//	//if (d == N)
//	//{
//	//	// solve Ax=b, overwriting b with the solution x 
//	//	ldl_lsolve(N, b, Lp, Li, Lx);
//	//	ldl_dsolve(N, b, D);
//	//	ldl_ltsolve(N, b, Lp, Li, Lx);
//	//	for (i = 0; i < N; i++) printf("x [%d] = %g\n", i, b[i]);
//	//}
//	//else
//	//{
//	//	printf("ldl_numeric failed, D (%d,%d) is zero\n", d, d);
//	//}
//
//
//	int a;
//	cin >> a;
//	return (0);
//	/* only the upper triangular part of A is required */
//
//	//int Ap2[3] = { 0,1,2 }, Ai2[5] = { 0,1,0,1,2 };
//	//double Ax2[5] = { 1,2,3,4 }, b2[2] = { 9,20 };
//	//double Lx2[5], D2[2], Y2[2];
//	//int Li2[5], Lp2[3], Parent2[2], Lnz2[2], Flag2[2], Pattern2[2], d2, i2;
//
//	//ldl_symbolic(2, Ap2, Ai2, Lp2, Parent2, Lnz2, Flag2, NULL, NULL);
//	//printf("Nonzeros in L, excluding diagonal: %d\n", Lp2[N]);
//	//d2 = ldl_numeric(2, Ap2, Ai2, Ax2, Lp2, Parent2, Lnz2, Li2, Lx2, D2, Y2, Pattern2,
//	//	Flag2, NULL, NULL);
//	//if (d2 == 2)
//	//{
//	//	/* solve Ax=b, overwriting b with the solution x */
//	//	ldl_lsolve(2, b2, Lp2, Li2, Lx2);
//	//	ldl_dsolve(2, b2, D2);
//	//	ldl_ltsolve(2, b2, Lp2, Li2, Lx2);
//	//	for (i2 = 0; i2 < 2; i2++) printf("x [%d] = %g\n", i2, b2[i2]);
//	//}
//	//else
//	//{
//	//	printf("ldl_numeric failed, D (%d,%d) is zero\n", d2, d2);
//	//}
//
//	//---------------------------------------------------
//
//}
