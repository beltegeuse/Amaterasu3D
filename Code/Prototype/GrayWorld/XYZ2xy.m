function xy = XYZ2xy(XYZ)
%Purpose: 
%	Calculate xy values from a matrix of XYZ values
%	XYZ values in the columns
%	
if size(XYZ,1) ~= 3
  error('XYZ values must be in the columns')
end

ncols = size(XYZ,2);
xy = zeros(2,ncols);

s = sum(XYZ);

xy(1,:) = XYZ(1,:) ./ s;

xy(2,:) = XYZ(2,:) ./ s;