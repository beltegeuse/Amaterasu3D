function GrayWorld(filename,outFile,catType,maxIter,plots)
%grayWorld(filename,outfile,catType,maxIter,plot)
% Performs color balancing via the gray world assumption and a chromatic
% adpatation transform.  Can be run interatively to possibly improve the
% result.  Set plot = 0 or 1 to turn diagnostic plots on or off.

%tutorialinit

%% import image
im_orig = imread(filename);
% figure
% imshow(im_orig)
% title('Original Image')

%% various constants
xyz_D65 = [95.04; 100; 108.88]; %http://en.wikipedia.org/wiki/D65, normalized Y = 100

sRGBtoXYZ =   [0.4124564  0.3575761  0.1804375; ...
               0.2126729  0.7151522  0.0721750; ...
               0.0193339  0.1191920  0.9503041];

%% grayworld
% catType = 'vonKries';
% maxIter = 1;
b = 0.001; %convergence threshold
iter = 1

imRGB_orig = (im_orig);
imRGB = imRGB_orig;

grayDiff =[];
rgbEst = mean(mean(imRGB,2)); %grayworld, average everything
grayDiff = [grayDiff norm([rgbEst(1)-rgbEst(2),rgbEst(1)-rgbEst(3),rgbEst(2)-rgbEst(3)])];

if grayDiff(end) < b
    disp(['Converged. RGB difference vector < ' num2str(b) ' in magnitude.'])
elseif iter >= 2 && abs(grayDiff(end-1)-grayDiff(end)) < 10^-6
    disp(['RGB difference vector no longer improving.'])
end

xyEst = XYZ2xy(sRGBtoXYZ*[rgbEst(1); rgbEst(2); rgbEst(3)]); %calculate xy chromaticity
xyzEst = xy2XYZ(xyEst,100); %normalize Y to 100 so D65 luminance comparable
trans = cbCAT(xyzEst,xyz_D65,catType)
for i=1:size(im_orig(:,:,1),1)
    for j=1:size(im_orig(:,:,1),2)
        imRGB(i,j,:) = uint8(trans*double([imRGB(i,j,1);imRGB(i,j,2);imRGB(i,j,3)]));
    end
end


imwrite(imRGB,outFile,'png');

if plots
    length(grayDiff) %number of iterations done
%     figure
%     imshow(cbunshape(imRGB,size(im_orig))/255)
%     title('Gray World Corrected')
    
    figure
    plot(grayDiff)
    title('GW: Norm of RGB Difference Vector vs Iterations')
    print(gcf,'-dpng',[outFile '-fig1'])
end
