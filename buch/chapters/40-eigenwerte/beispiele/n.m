#
# n.m -- Polynome mit dem gleichen Wert von p(A)
#
# (c) 2021 Prof Dr Andreas Müller, OST Ostschweizer Fachhochschule
#

A0 = [
  2, 1, 0;
  0, 2, 0;
  0, 0, 3
];

# find a 3x3 matrix in SL(3,Z)

function retval = zufallswert()
	x = round(rand() * 10) - 2;
	if (x >= 0)
		x = x + 1;
	endif
	retval = x;
end

function	retval = zufallsmatrix(n)
	retval = zeros(n, n);
	for i = (1:n)
		for j = (1:n)
			retval(i,j) = zufallswert();
		end
	end
end

function	retval = regulaer(n)
	d = 0;
	do
		retval = zufallsmatrix(2);
		d = det(retval);
	until (d == 1);
end

function	retval = eingebettet(n,k)
	retval = eye(n);
	retval(k:k+1,k:k+1) = regulaer(2);
end

format long

B = eye(3);
B = B * eingebettet(3,2)
B = B * eingebettet(3,1)

B
inverse(B)

A = round(B * A0 * inverse(B))

