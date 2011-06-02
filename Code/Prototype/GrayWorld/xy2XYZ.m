function cieXYZ = xy2XYZ(xy,Y)
%
%	Convert a xyY to XYZ
%

X = (xy(1,:) ./ xy(2,:)) .* Y;
Z = ((1 - xy(1,:) - xy(2,:)) ./ xy(2,:)) .* Y;
cieXYZ = [X; Y; Z];
