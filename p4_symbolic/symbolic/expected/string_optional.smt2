; 
(set-info :status unknown)
(declare-fun standard_metadata.ingress_port () (_ BitVec 9))
(declare-fun standard_metadata.egress_spec () (_ BitVec 9))
(declare-fun scalars.metadata.string_field () (_ BitVec 9))
(assert
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x84 (or (or (or false (= standard_metadata.ingress_port (_ bv0 9))) $x35) $x38)))
 (and (and (distinct standard_metadata.ingress_port (_ bv511 9)) true) $x84)))))
(assert
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let (($x40 (and true $x33)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (or $x49 (or (or (or false (= ?x80 (_ bv0 9))) (= ?x80 (_ bv1 9))) (= ?x80 (_ bv2 9)))))))))))))))))))))))
(assert
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let (($x40 (and true $x33)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let (($x69 (and true $x68)))
 (let ((?x79 (ite $x69 0 (ite (and true true) 1 (- 1)))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (and (and (not $x49) true) (= ?x79 (- 1)))))))))))))))))))))))
(check-sat)

; 
(set-info :status unknown)
(declare-fun standard_metadata.ingress_port () (_ BitVec 9))
(declare-fun standard_metadata.egress_spec () (_ BitVec 9))
(declare-fun scalars.metadata.string_field () (_ BitVec 9))
(assert
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x84 (or (or (or false (= standard_metadata.ingress_port (_ bv0 9))) $x35) $x38)))
 (and (and (distinct standard_metadata.ingress_port (_ bv511 9)) true) $x84)))))
(assert
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let (($x40 (and true $x33)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (or $x49 (or (or (or false (= ?x80 (_ bv0 9))) (= ?x80 (_ bv1 9))) (= ?x80 (_ bv2 9)))))))))))))))))))))))
(assert
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let (($x40 (and true $x33)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let (($x69 (and true $x68)))
 (let ((?x79 (ite $x69 0 (ite (and true true) 1 (- 1)))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (let (($x142 (and (not $x49) true)))
 (and $x142 (= ?x79 0)))))))))))))))))))))))
(check-sat)

; 
(set-info :status unknown)
(declare-fun standard_metadata.ingress_port () (_ BitVec 9))
(declare-fun standard_metadata.egress_spec () (_ BitVec 9))
(declare-fun scalars.metadata.string_field () (_ BitVec 9))
(assert
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x84 (or (or (or false (= standard_metadata.ingress_port (_ bv0 9))) $x35) $x38)))
 (and (and (distinct standard_metadata.ingress_port (_ bv511 9)) true) $x84)))))
(assert
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let (($x40 (and true $x33)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (or $x49 (or (or (or false (= ?x80 (_ bv0 9))) (= ?x80 (_ bv1 9))) (= ?x80 (_ bv2 9)))))))))))))))))))))))
(assert
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let (($x40 (and true $x33)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let (($x69 (and true $x68)))
 (let ((?x79 (ite $x69 0 (ite (and true true) 1 (- 1)))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (and (and (not $x49) true) (= ?x79 1))))))))))))))))))))))
(check-sat)

; 
(set-info :status unknown)
(declare-fun standard_metadata.ingress_port () (_ BitVec 9))
(declare-fun standard_metadata.egress_spec () (_ BitVec 9))
(declare-fun scalars.metadata.string_field () (_ BitVec 9))
(assert
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x84 (or (or (or false (= standard_metadata.ingress_port (_ bv0 9))) $x35) $x38)))
 (and (and (distinct standard_metadata.ingress_port (_ bv511 9)) true) $x84)))))
(assert
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let (($x40 (and true $x33)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (or $x49 (or (or (or false (= ?x80 (_ bv0 9))) (= ?x80 (_ bv1 9))) (= ?x80 (_ bv2 9)))))))))))))))))))))))
(assert
 (let ((?x53 (ite (and true (and true (= standard_metadata.ingress_port (_ bv2 9)))) 2 (- 1))))
 (let ((?x57 (ite (and true (and true (= standard_metadata.ingress_port (_ bv1 9)))) 1 ?x53)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x40 (and true $x33)))
 (let ((?x62 (ite $x40 0 ?x57)))
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (and (and (not $x49) true) (= ?x62 (- 1)))))))))))))))))))))))))
(check-sat)

; 
(set-info :status unknown)
(declare-fun standard_metadata.ingress_port () (_ BitVec 9))
(declare-fun standard_metadata.egress_spec () (_ BitVec 9))
(declare-fun scalars.metadata.string_field () (_ BitVec 9))
(assert
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x84 (or (or (or false (= standard_metadata.ingress_port (_ bv0 9))) $x35) $x38)))
 (and (and (distinct standard_metadata.ingress_port (_ bv511 9)) true) $x84)))))
(assert
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let (($x40 (and true $x33)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (or $x49 (or (or (or false (= ?x80 (_ bv0 9))) (= ?x80 (_ bv1 9))) (= ?x80 (_ bv2 9)))))))))))))))))))))))
(assert
 (let ((?x53 (ite (and true (and true (= standard_metadata.ingress_port (_ bv2 9)))) 2 (- 1))))
 (let ((?x57 (ite (and true (and true (= standard_metadata.ingress_port (_ bv1 9)))) 1 ?x53)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x40 (and true $x33)))
 (let ((?x62 (ite $x40 0 ?x57)))
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (let (($x142 (and (not $x49) true)))
 (and $x142 (= ?x62 0)))))))))))))))))))))))))
(check-sat)

; 
(set-info :status unknown)
(declare-fun standard_metadata.ingress_port () (_ BitVec 9))
(declare-fun standard_metadata.egress_spec () (_ BitVec 9))
(declare-fun scalars.metadata.string_field () (_ BitVec 9))
(assert
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x84 (or (or (or false (= standard_metadata.ingress_port (_ bv0 9))) $x35) $x38)))
 (and (and (distinct standard_metadata.ingress_port (_ bv511 9)) true) $x84)))))
(assert
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let (($x40 (and true $x33)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (or $x49 (or (or (or false (= ?x80 (_ bv0 9))) (= ?x80 (_ bv1 9))) (= ?x80 (_ bv2 9)))))))))))))))))))))))
(assert
 (let ((?x53 (ite (and true (and true (= standard_metadata.ingress_port (_ bv2 9)))) 2 (- 1))))
 (let ((?x57 (ite (and true (and true (= standard_metadata.ingress_port (_ bv1 9)))) 1 ?x53)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x40 (and true $x33)))
 (let ((?x62 (ite $x40 0 ?x57)))
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (and (and (not $x49) true) (= ?x62 1))))))))))))))))))))))))
(check-sat)

; 
(set-info :status unknown)
(declare-fun standard_metadata.ingress_port () (_ BitVec 9))
(declare-fun standard_metadata.egress_spec () (_ BitVec 9))
(declare-fun scalars.metadata.string_field () (_ BitVec 9))
(assert
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x84 (or (or (or false (= standard_metadata.ingress_port (_ bv0 9))) $x35) $x38)))
 (and (and (distinct standard_metadata.ingress_port (_ bv511 9)) true) $x84)))))
(assert
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let (($x40 (and true $x33)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (or $x49 (or (or (or false (= ?x80 (_ bv0 9))) (= ?x80 (_ bv1 9))) (= ?x80 (_ bv2 9)))))))))))))))))))))))
(assert
 (let ((?x53 (ite (and true (and true (= standard_metadata.ingress_port (_ bv2 9)))) 2 (- 1))))
 (let ((?x57 (ite (and true (and true (= standard_metadata.ingress_port (_ bv1 9)))) 1 ?x53)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x40 (and true $x33)))
 (let ((?x62 (ite $x40 0 ?x57)))
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (and (and (not $x49) true) (= ?x62 2))))))))))))))))))))))))
(check-sat)

; 
(set-info :status unknown)
(declare-fun standard_metadata.ingress_port () (_ BitVec 9))
(declare-fun standard_metadata.egress_spec () (_ BitVec 9))
(declare-fun scalars.metadata.string_field () (_ BitVec 9))
(assert
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x84 (or (or (or false (= standard_metadata.ingress_port (_ bv0 9))) $x35) $x38)))
 (and (and (distinct standard_metadata.ingress_port (_ bv511 9)) true) $x84)))))
(assert
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let (($x40 (and true $x33)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (or $x49 (or (or (or false (= ?x80 (_ bv0 9))) (= ?x80 (_ bv1 9))) (= ?x80 (_ bv2 9)))))))))))))))))))))))
(assert
 (let ((?x65 (concat (_ bv0 7) (_ bv2 2))))
 (let ((?x29 (concat (_ bv0 8) (_ bv0 1))))
 (let (($x38 (= standard_metadata.ingress_port (_ bv2 9))))
 (let (($x39 (and true $x38)))
 (let (($x32 (= standard_metadata.ingress_port (_ bv0 9))))
 (let (($x33 (and true $x32)))
 (let (($x41 (not $x33)))
 (let (($x46 (and true (and $x41 (not (and true (= standard_metadata.ingress_port (_ bv1 9))))))))
 (let ((?x54 (ite (and $x46 $x39) ?x29 (ite true ?x29 scalars.metadata.string_field))))
 (let (($x35 (= standard_metadata.ingress_port (_ bv1 9))))
 (let (($x36 (and true $x35)))
 (let (($x40 (and true $x33)))
 (let ((?x66 (ite $x40 ?x65 (ite (and (and true $x41) $x36) (concat (_ bv0 8) (_ bv1 1)) ?x54))))
 (let (($x68 (and true (= ?x66 ?x65))))
 (let ((?x78 (ite (and (and true (not $x68)) true) (_ bv1 9) standard_metadata.egress_spec)))
 (let (($x69 (and true $x68)))
 (let ((?x80 (ite $x69 (_ bv0 9) ?x78)))
 (let (($x49 (= ?x80 (_ bv511 9))))
 (and (and (not $x49) true) (= (- 1) (- 1))))))))))))))))))))))
(check-sat)

