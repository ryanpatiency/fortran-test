program outputdata
   implicit none

   real, dimension(100) :: x, y
   real, dimension(100) :: p, q
   integer :: i
   ! data
   do i = 1,10
      x(i) = i * 0.1
      y(i) = sin(x(i)) * (1-cos(x(i)/3.0))
   end do
   
   ! output data into a file
   open(1, file = 'data1.dat', status='old', access = "append", &
   form="formatted", blank="null", action="write", &
   iostat=i, recl=8, position="append" )
   do i = 1,10
      write(1,*) x(i), y(i)
   end do
   close(1)

   ! opening the file for reading
   open (2, file = 'data1.dat', status = 'old')

   do i = 1,10
      read(2,*) p(i), q(i)
   end do


   do i = 1,10
      write(*,*) p(i), q(i)
   end do
   ! read(*,*)

end
