using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ButtonCollider : MonoBehaviour
{
    private void OnTriggerStay(Collider other) {
        if (GameManager.Instance.start && !GameManager.Instance.loseCondition) {
            if (other.CompareTag("Player") || other.CompareTag("Crate")) {
                gameObject.transform.localPosition = Vector3.zero;
                GameManager.Instance.buttCondition = true;
            }
        }
    }

    private void OnTriggerExit(Collider other) {
        if (GameManager.Instance.start) {
            if (other.CompareTag("Player") || other.CompareTag("Crate")) {
                gameObject.transform.localPosition = new Vector3(0, 0.35f, 0);
                GameManager.Instance.buttCondition = false;
            }
        }
    }
}
