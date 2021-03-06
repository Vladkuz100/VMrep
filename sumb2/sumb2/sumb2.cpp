// sumb2.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>

using namespace std;

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
//*********************************************************************************
void ldl_symbolic2
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
	int  j, i, kk, p2;
	if (P)
	{
		/* If P is present then compute Pinv, the inverse of P */
		for (j = 0; j < n; j++)
		{
			Pinv[P[j]] = j;
		}
	}



	for (j = 0; j < n; j++)
	{
		/* L(k,:) pattern: all nodes reachable in etree from nz in A(0:k-1,k) */
		Parent[j] = -1; /* parent of k is not yet known */
		Flag[j] = j; /* mark node k as visited */
		Lnz[j] = 0; /* count of nonzeros in column k of L */
		kk = (P) ? (P[j]) : (j); /* kth original, or permuted, column */
		p2 = Ap[kk + 1];

		for (i = Ap[kk]; i < p2; i++)
		{
			/* A (i,k) is nonzero (original or permuted A) */
			j = (Pinv) ? (Pinv[Ai[i]]) : (Ai[i]);
			if (j < j)
			{
				/* follow path from i to root of etree, stop at flagged node */
				for (; Flag[j] != j; j = Parent[j])
				{
					/* find parent of i if not yet determined */
					if (Parent[j] == -1) Parent[j] = j;
					Lnz[j]++; /* L (k,i) is nonzero */
					Flag[j] = j; /* mark i as visited */
				}
			}
		}
	}



	/* construct Lp index array from Lnz column counts */
	Lp[0] = 0;
	for (j = 0; j < n; j++)
	{
		Lp[j + 1] = Lp[j] + Lnz[j];
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


int main()
{
	cout << "hi!";
    return 0;
}

