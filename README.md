# asu_math_lab
Software Engineering Project for 3rd year Computer and Systems Engineering department at Ain Shams University


in main there will be a function named : take_input()
this function will take the input from the user, give this function to a function named remove_spaces()
to remove any un-needed spaces in the input string.

 then determine if it is a matrix definition using brackets or ones or eyes or rand
or it is an expression involving numbers or an expression involving matrices operation

1) if it is matrix definition by brackets then take_input will call another function named : create_matrix()
     and push this matrix in a vector of matrices
2) if input is matrix operations then it will call a function named: calculate_expression() that will calculate the resulting matrix 
     and store it in the matrix vector
3) if input is operations on numbers only, it will call a function named calculate()
