-- Name: Abhishek Shaw
-- Roll No.: 19CS60R48
-- Question 1:Find the most productive month for supplier company named “Leka Trading” in year 2013.

select r.Cname as CompanyName,SUBSTRING(UPPER(r.Mname),1,3) as Month,r.Summation as TotalAmount
from 
(select CompanyName as Cname,MONTHNAME(Orders.OrderDate) as Mname ,sum(unitPrice*Quantity) as Summation
from ((Product natural join Supplier)
natural join OrderItem)
natural join Orders
where CompanyName="Leka Trading" and YEAR(Orders.OrderDate)=2013
group by MONTHNAME(Orders.OrderDate)
order by sum(unitPrice*Quantity) DESC
LIMIT 1)r;