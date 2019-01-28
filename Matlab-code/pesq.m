% ----------------------------------------------------------------------
%                            Disclamer
%
%   This p-coded version of the PESQ measure is provided here
%   by permission from the authors. The m-file sources for the 
%   PESQ measure can be found in [1]. 
%
%   You can also visit authors website at: 
%   http://www.utdallas.edu/~loizou/speech/software.htm
%
% ----------------------------------------------------------------------
%              PESQ objective speech quality measure
%
%   This function implements the PESQ measure based on the ITU standard
%   P.862 [2].
%
%   Usage:  pval=pesq(clean, enhanced, fs)
%         
%         clean    - clean speech vector
%         enhanced - enhanced speech vector
%         fs       - sampling frequency
%         pval     - PESQ value
%
%    Note that the PESQ routine only supports sampling rates of 8 kHz and
%    16 kHz [2]
%  
%  References:
%   [1] Loizou, P. (2007). Speech Enhancement: Theory and Practice,
%       CRC Press, Boca Raton: FL.
%   [2] ITU (2000). Perceptual evaluation of speech quality (PESQ), and 
%       objective method for end-to-end speech quality assessment of 
%       narrowband telephone networks and speech codecs. ITU-T
%       Recommendation P. 862   
%
%   Authors: Yi Hu and Philipos C. Loizou 
%
% Copyright (c) 2006 by Philipos C. Loizou
% $Revision: 0.0 $  $Date: 10/09/2006 $
% ----------------------------------------------------------------------
