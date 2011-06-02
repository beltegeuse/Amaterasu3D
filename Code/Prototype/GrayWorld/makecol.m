function [Column]=makecol(data,formatno);

if nargin<2
  formatno=4;
end

[NoRows, NoCols]=size(data);

Column=num2str(data(1),formatno);
for gapli=2:NoRows
  Column=strvcat(Column,num2str(data(gapli),formatno));
end