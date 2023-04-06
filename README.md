1. a) 


P(Ci):

P(26k..30k) = 46/165
P(31k..35k) = 40/165
P(36k..40k) = 4/165
P(41k..45k) = 4/165
P(46k..50k) = 63/165
P(66k..70k) = 8/165

P(xk | Ci):

P(Department=Sales | Salary = 46k..50k) = 30/63
P(Status=senior | Salary = 46k..50k) = 40/63
P(Age=31..35 | Salary = 46k..50k) = 30/63
P(Department=Sales | Salary = 26k..30k) = 40/46
P(Status=junior | Salary = 26k..30k) = 46/46
P(Age=26..30 | Salary = 26k..30k) = 46/46
P(Department=Sales | Salary = 31k..35k) = 40/40
P(Status=junior | Salary = 31k..35k) = 40/40
P(Age=31..35 | Salary = 31k..35k) = 40/40
P(Department=Systems | Salary = 46k..50k) = 23/63
P(Status=junior | Salary = 46k..50k) = 23/63
P(Age=21..25 | Salary = 46k..50k) = 20/63
P(Department=Systems | Salary = 66k..70k) = 8/8
P(Status=senior | Salary = 66k..70k) = 8/8
P(Age=31..35 | Salary = 66k..70k) = 5/8
P(Age=26..30 | Salary = 46k..50k) = 3/63
P(Age=41..45 | Salary = 66k..70k) = 3/8
P(Department=Marketing | Salary = 46k..50k) = 10/63
P(Age=36..40 | Salary = 46k..50k) = 10/63
P(Department=Marketing | Salary = 41k..45k) = 4/4
P(Status=junior | Salary = 41k..45k) = 4/4
P(Age=31..35 | Salary = 41k..45k) = 4/4
P(Department=Secretary | Salary = 36k..40k) = 4/4
P(Status=senior | Salary = 36k..40k) = 4/4
P(Age=46..50 | Salary = 36k..40k) = 4/4
P(Department=Secretary | Salary = 26k..30k) = 6/46
* Note: all 0 probabilites are not shown

1. b)

P(Ci):

P(26k..30k) = 46/165
P(31k..35k) = 40/165
P(36k..40k) = 4/165
P(41k..45k) = 4/165
P(46k..50k) = 63/165
P(66k..70k) = 8/165

need to maximize P(X|Ci)P(Ci)

X = (Department =Systems, Status = junior, Age=26..30)

P(Department = Systems | Salary = 26k..30k) = 0
P(Department = Systems | Salary = 31k..35k) = 0
P(Department = Systems | Salary = 36k..40k) = 0
P(Department = Systems | Salary = 41k..45k) = 0
P(Department = Systems | Salary = 46k..50k) = 23/63
P(Department = Systems | Salary = 66k..70k) = 8/8

P(Status = junior | Salary = 26k..30k) = 46/46
P(Status = junior | Salary = 31k..35k) = 40/40
P(Status = junior | Salary = 36k..40k) = 0
P(Status = junior | Salary = 41k..45k) = 4/4
P(Status = junior | Salary = 46k..50k) = 23/63
P(Status = junior | Salary = 66k..70k) = 0

P(Age=26..30 | Salary = 26k..30k) = 46/46
P(Age=26..30| Salary = 31k..35k) = 0
P(Age=26..30 | Salary = 36k..40k) = 0
P(Age=26..30| Salary = 41k..45k) = 0
P(Age=26..30 | Salary = 46k..50k) = 3/63
P(Age=26..30 | Salary = 66k..70k) = 0


P(X|Ci):

P(X | 26k..30k) = 0
P(X | 31k..35k) = 0
P(X | 36k..40k) = 0
P(X | 41k..45k) = 0
P(X | 46k..50k) = 3/63  x  23/63   x 23/63
P(X | 66k..70k) = 0

without further ado we can classify salary range as 46k..50k because all P(X|Ci)P(Ci) are 0 except that one, hence it is the max

1. c)

For these classes it's 0:

(salary = 26k..30k)
(salary = 31k..35k)
(salary = 36k..40k)
(salary = 41k..45k)
(salary = 66k..70k)

This is not good because classifier will perform bad for new data instances with (Department =Systems, Status = junior, Age=26..30)


2. a)
S.D. = (100 * 0.17(0.83))^(1/2)
     = 3.76

confidence = 0.17 +- (p * (1 - p) / N) ^ (1/2) * 1.96
           = 0.17 +- 0.074

2. b)
    (p * (1 - p) / N) ^ (1/2) * 1.96 <= 0.05
    * rearrange *
    N >= 217

           